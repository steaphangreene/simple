// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>

using namespace std;

#include "sg.h"
#include "sg_alignment.h"
#include "sg_positional.h"
#include "sg_events.h"
#include "simpletexture.h"

SimpleGUI *current_sg = NULL;

Uint32 SDL_SG_EVENT = Uint32(-1);

SimpleGUI::SimpleGUI(int aspmeth, const float asp) {
  if(current_sg) {
    fprintf(stderr, "ERROR: Attempted to create multiple SimpleGUI instances.\n");
    exit(1);
    }
  current_sg = this;

  SDL_SG_EVENT = SDL_RegisterEvents(1);
  if (SDL_SG_EVENT == ((Uint32)-1)) {
    fprintf(stderr, "ERROR: Failed to allocate SDL event.\n");
    exit(1);
    }

  current_widget = NULL;
  focus_widget = NULL;
  mutex = NULL;

  flags = 0;

  aspect = asp;
  aspect_actual = 1.0;
  aspect_method = aspmeth;
  newaspect_actual = aspect_actual;

  mWid = new SG_Positional;
  mWid->SetAspectRatio(asp);
  mainWid = new SG_Alignment;
  mWid->SetBackground(mainWid);

  popWid = NULL;
  pop_modal = false;

  int screen_geom[4];
  glGetIntegerv(GL_VIEWPORT, screen_geom);
  xunused = screen_geom[0]*2;
  yunused = screen_geom[1]*2;
  xsize = screen_geom[2];
  ysize = screen_geom[3];
  newxsize = xsize;
  newysize = ysize;
  newxunused = xunused;
  newyunused = yunused;

  SDL_ShowCursor(0);

  mb_state = 0;

  mousex = -100.0;	// So system knows not to draw cursor until it moves
  mousey = -100.0;
  mouse_cursor = NULL;
  mouse_xscale = 0.0625;
  mouse_yscale = 0.0625;

  SimpleTexture::SetColor(SG_COL_BG,     0.500, 0.500, 0.500);
  SimpleTexture::SetColor(SG_COL_FG,     0.625, 0.625, 0.625);
  SimpleTexture::SetColor(SG_COL_RAISED, 0.750, 0.750, 0.750);
  SimpleTexture::SetColor(SG_COL_LOW,    0.375, 0.375, 0.375);
  SimpleTexture::SetColor(SG_COL_HIGH,   0.625, 0.625, 1.000);
  }

SimpleGUI::~SimpleGUI() {
  SimpleTexture::EmptyTrash();
  glFinish();

  delete mWid;
  mWid = NULL;

  if(mutex) SDL_DestroyMutex(mutex);
  mutex = NULL;
  }

bool SimpleGUI::Render(unsigned long cur_time) {
  if(!RenderStart(cur_time)) return 0;
  return RenderFinish(cur_time);
  }

bool SimpleGUI::RenderStart(unsigned long cur_time, bool ts) {
  if(ts) SDL_mutexP(Mutex());
  if(newxunused != xunused || newyunused != yunused || newxsize != xsize
	|| newysize != ysize || newaspect_actual != aspect_actual) {
    xunused = newxunused;
    yunused = newyunused;
    xsize = newxsize;
    ysize = newysize;
    aspect_actual = newaspect_actual;
    }
  if(ts) SDL_mutexV(Mutex());
  return 1;
  }

bool SimpleGUI::RenderFinish(unsigned long cur_time, bool ts) {
  if(ts) SDL_mutexP(Mutex());

  //This basically leverages the perspective to flat 2D
  //with a -1.0 to 1.0 centered coord system
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  //This makes sure the needed functions are setup right for the widgets.
  glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);

  if(aspect_actual > 1.0 && (aspect_method & ASPECT_EXPANDING_X)) {
    glScalef(1.0/aspect_actual, 1.0, 1.0);
    }
  else if(aspect_actual < 1.0 && (aspect_method & ASPECT_EXPANDING_Y)) {
    glScalef(1.0, aspect_actual, 1.0);
    }

  matrix_saved = false;
  mWid->Render(cur_time);
  if(current_widget && matrix_saved) {
    glPushMatrix();
    glLoadMatrixd(saved_matrix);
    current_widget->Render(cur_time, true);
    glPopMatrix();
    }

  //Now we draw the mouse cursor
  //- if it's enabled and at least one axis is within coord system
  if(((mousex >= -1.0 && mousex <= 1.0) || (mousey >= -1.0 && mousey <= 1.0))
	&& (flags & SIMPLEGUI_NOMOUSE) == 0) {
    glTranslatef(mousex, mousey, 0.5);
    glScalef(mouse_xscale/aspect, mouse_yscale, 1.0);

    if(!mouse_cursor) {		//Just a square cursor for now
      //Just to show how basic animation could work
      float glow = (sin((float)(cur_time)/200.0f) + 1.0f) / 2.0f;
      glColor3f(glow, glow, 1.0f - glow);

      glBegin(GL_QUADS);
      glVertex3f(0.0, -1.0, 0.0);
      glVertex3f(1.0, -1.0, 0.0);
      glVertex3f(1.0,  0.0, 0.0);
      glVertex3f(0.0,  0.0, 0.0);
      glEnd();
      }
    else {			//Draw a real cursor
      glEnable(GL_BLEND);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, mouse_cursor->GLTexture());
      glColor3f(1.0f, 1.0f, 1.0f);

      glBegin(GL_QUADS);
      glTexCoord2f(mouse_cursor->ScaleX(0.0), mouse_cursor->ScaleY(1.0));
      glVertex3f(-1.0, -1.0, 0.0);
      glTexCoord2f(mouse_cursor->ScaleX(1.0), mouse_cursor->ScaleY(1.0));
      glVertex3f( 1.0, -1.0, 0.0);
      glTexCoord2f(mouse_cursor->ScaleX(1.0), mouse_cursor->ScaleY(0.0));
      glVertex3f( 1.0, 1.0, 0.0);
      glTexCoord2f(mouse_cursor->ScaleX(0.0), mouse_cursor->ScaleY(0.0));
      glVertex3f(-1.0, 1.0, 0.0);
      glEnd();

      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
      }
    }

  //This puts the perspective back where it was
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  if(ts) {
    SDL_mutexV(Mutex());
    SDL_PumpEvents();
    }

  SimpleTexture::EmptyTrash();
  return 1;
  }

bool SimpleGUI::PollEvent(SDL_Event *event, bool ts) {
  if(!ts) SDL_PumpEvents();	//Only pump once!
  while(SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT) > 0) {
    if(event == NULL) return true;
    else {
      if(ts) SDL_mutexP(Mutex());
      bool res = ProcessEvent(event);
      if(ts) SDL_mutexV(Mutex());
      if(res && event->type == SDL_SG_EVENT
		&& event->user.code == SG_EVENT_NEEDTORENDER) res = false;
      if(res) return true;
      }
    }
  return false;
  }

bool SimpleGUI::WaitEvent(SDL_Event *event, bool ts) {
  if(!ts) SDL_PumpEvents();	//Pump once before everything...
  int res = SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
  while(res >= 0) {
    if(res > 0) {	//If there IS an event here or ready
      if(event == NULL) return true;
      else {
	if(ts) SDL_mutexP(Mutex());
	bool res = ProcessEvent(event);
	if(ts) SDL_mutexV(Mutex());
	if(res) return true;
	}
      }
    SDL_Delay(10);
    if(!ts) SDL_PumpEvents();	//...and pump after each wait.
    res = SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    }
  return false;
  }

bool SimpleGUI::ProcessEvent(SDL_Event *event) {
  if(!event) return 0;

  if(event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
    newxunused = 0;
    newyunused = 0;
    newxsize = event->window.data1;
    newysize = event->window.data2;

    float asp = (float)(newxsize) / (float)(newysize);
    if((aspect_method & ASPECT_FIXED_X) && asp > aspect) {
      newxsize = int((float)(newysize)*aspect+0.5);
      newxunused = (event->window.data1 - newxsize);
      }
    else if((aspect_method & ASPECT_FIXED_Y) && asp < aspect) {
      newysize = int((float)(newxsize)/aspect+0.5);
      newyunused = (event->window.data2 - newysize);
      }
    newaspect_actual = asp / aspect;
    SimpleTexture::NeedToReacquireContext(event->window.data1, event->window.data2);
    if((aspect_method & (ASPECT_FIXED_Y|ASPECT_FIXED_X))
	!= (ASPECT_FIXED_Y|ASPECT_FIXED_X)) {
      if(mWid) mWid->SetAspectRatio(asp);
      }
    return 0; //Event Handled
    }

  else if(event->type == SDL_TEXTINPUT) {
    if(focus_widget) {
      if(mWid->HasWidget(focus_widget)) {
	return mWid->HandEventTo(focus_widget, event, mousex, mousey);
	}
      focus_widget->HandEventTo(focus_widget, event, 0.0, 0.0);
      }
    }

  if(flags & SIMPLEGUI_NOMOUSE) return 1;	//No Mouse Events!

  if(event->type == SDL_MOUSEBUTTONDOWN) {
    mb_state |= SDL_BUTTON(event->button.button);
    if(current_widget) {
      mousex = float(event->button.x);
      mousey = float(event->button.y);
      ScreenToRelative(mousex, mousey);
      int ret = mWid->HandEventTo(current_widget, event, mousex, mousey);
      if(current_widget && ret && event->type != SDL_SG_EVENT) {
	ret = current_widget->HandleEvent(event, 0.0, 0.0);
	}
      if(ret) return ret;
      }

    focus_widget = NULL;	// They must reclaim this if it's still theirs

    mousex = float(event->button.x);
    mousey = float(event->button.y);
    ScreenToRelative(mousex, mousey);

    return mWid->HandleEvent(event, mousex, mousey);
    }

  else if(event->type == SDL_MOUSEBUTTONUP) {
    mb_state &= (~SDL_BUTTON(event->button.button));
    if(current_widget) {
      if(mb_state) return 0;	//Ignore if any button is still pressed

      mousex = float(event->button.x);
      mousey = float(event->button.y);
      ScreenToRelative(mousex, mousey);

      SG_Widget *cur = current_widget;
      current_widget = NULL;
      int ret = 0;
      ret = mWid->HandEventTo(cur, event, mousex, mousey);
      if(ret && event->type != SDL_SG_EVENT) {
	ret = cur->HandleEvent(event, 0.0, 0.0);
	}
      return ret;
      }
    }

  else if(event->type == SDL_MOUSEMOTION) {
    mousex = float(event->motion.x);
    mousey = float(event->motion.y);
    ScreenToRelative(mousex, mousey);

    int ret = 1;
    if(current_widget) {
      ret = mWid->HandEventTo(current_widget, event, mousex, mousey);
      if(current_widget && ret && event->type != SDL_SG_EVENT) {
	ret = current_widget->HandleEvent(event, 0.0, 0.0);
	}
      }

    if(ret && event->type != SDL_SG_EVENT)
      ret = mWid->HandleEvent(event, mousex, mousey);

    if(!ret) {	//Mouse DID move, so render is needed
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_NEEDTORENDER;
      ret = 1;
      }

    return ret;
    }

  return 1;
  }

int SimpleGUI::ScreenToRelative(float &x, float &y) {
//  fprintf(stderr, "(%f,%f) on (%d,%d):%dx%d\n", x, y,
//	xunused, yunused, xsize, ysize);

  if(xsize <= 0 || ysize <= 0 || xunused < 0 || yunused < 0) { //Invalid
    return 0;
    }

  int ret = 1;

  x -= (float)(xunused/2);
  y -= (float)(yunused/2);

  if(x < 0.0 || y < 0.0 || x >= (float)(xsize) || y >= (float)(ysize)) {
    ret = 0;
    }

  x += 0.5;
  y += 0.5;

  x /= (float)(xsize);
  y /= (float)(ysize);

  x *= 2.0;
  y *= -2.0;

  x -= 1.0;
  y += 1.0;

  if(aspect_actual > 1.0 && (aspect_method & ASPECT_EXPANDING_X)) {
    x *= aspect_actual;
    }
  else if(aspect_actual < 1.0 && (aspect_method & ASPECT_EXPANDING_Y)) {
    y /= aspect_actual;
    }

  return ret;
  }

void SimpleGUI::SetFont(TTF_Font *font) {
  SimpleTexture::SetFont(font);
  }

void SimpleGUI::SetDefaultFontSize(int pxsz) {
  SimpleTexture::SetDefaultFontSize(pxsz);
  }

void SimpleGUI::LoadFont(const char *fontfn, int pxsz) {
  SimpleTexture::LoadFont(fontfn, pxsz);
  }

const TTF_Font *SimpleGUI::Font(int pxsz) {
  return SimpleTexture::Font(pxsz);
  }

void SimpleGUI::SetPopupWidget(SG_Alignment *wid, float px, float py,
	float posx, float posy) {
  if(pop_modal) mainWid->Listen();
  if(popWid) mWid->RemoveWidget(popWid);
  popWid = wid;
  if(popWid) mWid->AddWidget(popWid, posx, posy, px, py, pop_modal);
//  if(popWid) {
//    mWid->AddWidget(popWid);
//    mWid->SetBorder(1.0 - px, 1.0 - py);
//    }
  }

void SimpleGUI::SetModalPopupWidget(SG_Alignment *wid, float px, float py,
	float posx, float posy) {
  SetPopupWidget(wid, px, py, posx, posy);
  pop_modal = true;
  mainWid->Ignore();
  }

void SimpleGUI::SetMouseCursor(SDL_Surface *cur, float xsc, float ysc) {
  if(mouse_cursor) delete mouse_cursor;
  mouse_cursor = new SimpleTexture(cur);
  }

SimpleGUI *SimpleGUI::Current() {
  return current_sg;
  }

SDL_mutex *SimpleGUI::Mutex() {
  if(!mutex) mutex = SDL_CreateMutex();
  return mutex;
  }

float SimpleGUI::Red(int c) {
  return SimpleTexture::Red(c);
  }

float SimpleGUI::Green(int c) {
  return SimpleTexture::Green(c);
  }

float SimpleGUI::Blue(int c) {
  return SimpleTexture::Blue(c);
  }

float SimpleGUI::TextRed(int c) {
  return SimpleTexture::TextRed(c);
  }

float SimpleGUI::TextGreen(int c) {
  return SimpleTexture::TextGreen(c);
  }

float SimpleGUI::TextBlue(int c) {
  return SimpleTexture::TextBlue(c);
  }

const SDL_Color *SimpleGUI::BGColor(int c) {
  return SimpleTexture::BGColor(c);
  }

const SDL_Color *SimpleGUI::TextColor(int c) {
  return SimpleTexture::TextColor(c);
  }

void SimpleGUI::SetDefaultTextColor(float tr, float tg, float tb) {
  SimpleTexture::SetDefaultTextColor(tr, tg, tb);
  }

const SDL_Color *SimpleGUI::DefaultTextColor() {
  return SimpleTexture::DefaultTextColor();
  }

void SimpleGUI::SetColor(int c, float r, float g, float b,
	float tr, float tg, float tb) {
  SimpleTexture::SetColor(c, r, g, b, tr, tg, tb);
  }

int SimpleGUI::NewColor() {
  return SimpleTexture::NewColor();
  }

int SimpleGUI::NewColor(float r, float g, float b,
	float tr, float tg, float tb) {
  return SimpleTexture::NewColor(r, g, b, tr, tg, tb);
  }

SG_Alignment *SimpleGUI::MasterWidget() {
  return mainWid;
  }

SG_Alignment *SimpleGUI::PopupWidget() {
  return popWid;
  }

void SimpleGUI::SaveCurrentMatrix() {
  glGetDoublev(GL_MODELVIEW_MATRIX, saved_matrix);
  matrix_saved = true;
  }
