// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>

using namespace std;

#include "sg.h"
#include "sg_alignment.h"
#include "sg_events.h"
#include "sg_texture.h"

SimpleGUI *current_sg = NULL;

SimpleGUI::SimpleGUI(int aspmeth, float asp) {
  if(current_sg) {
    fprintf(stderr, "ERROR: Attempted to create multiple SimpleGUI instances.\n");
    exit(1);
    }
  current_sg = this;
  current_widget = NULL;
  focus_widget = NULL;
  mutex = NULL;

  flags = 0;

  mWid = new SG_Alignment();
  popWid = NULL;
  popx = 0.5, popy = 0.5;
  popxpos = 0.0, popypos = 0.0;
  pop_modal = false;

  aspect = asp;
  aspect_actual = 1.0;
  aspect_method = aspmeth;
  newaspect_actual = aspect_actual;

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
  SDL_EnableUNICODE(1);

  mb_state = 0;

  mousex = -100.0;	// So system knows not to draw cursor until it moves
  mousey = -100.0;
  mouse_cursor = NULL;
  mouse_xscale = 0.0625;
  mouse_yscale = 0.0625;

  col.resize(SG_COL_MAX * 2);

  text_col.r = 0;
  text_col.g = 0;
  text_col.b = 0;

  col[SG_COL_BG*2 + 0].r = 120;
  col[SG_COL_BG*2 + 0].g = 120;
  col[SG_COL_BG*2 + 0].b = 120;
  col[SG_COL_BG*2 + 1] = text_col;

  col[SG_COL_FG*2 + 0].r = 160;
  col[SG_COL_FG*2 + 0].g = 160;
  col[SG_COL_FG*2 + 0].b = 160;
  col[SG_COL_FG*2 + 1] = text_col;

  col[SG_COL_RAISED*2 + 0].r = 200;
  col[SG_COL_RAISED*2 + 0].g = 200;
  col[SG_COL_RAISED*2 + 0].b = 200;
  col[SG_COL_RAISED*2 + 1] = text_col;

  col[SG_COL_LOW*2 + 0].r = 90;
  col[SG_COL_LOW*2 + 0].g = 90;
  col[SG_COL_LOW*2 + 0].b = 90;
  col[SG_COL_LOW*2 + 1] = text_col;

  col[SG_COL_HIGH*2 + 0].r = 160;
  col[SG_COL_HIGH*2 + 0].g = 160;
  col[SG_COL_HIGH*2 + 0].b = 250;
  col[SG_COL_HIGH*2 + 1] = text_col;

  fontfile = NULL;
  fontyratio = 1.0;
  }

SimpleGUI::~SimpleGUI() {
  SG_Texture::EmptyTrash();
  glFinish();

  delete mWid;
  mWid = NULL;
  if(popWid) delete popWid;
  popWid = NULL;

  if(fontfile) {		//Only close if WE opened
    delete [] fontfile;
    fontfile = NULL;
    map<int, TTF_Font *>::iterator itrf = cur_font.begin();
    for(; itrf != cur_font.end(); ++itrf) {
      TTF_CloseFont(itrf->second);
      }
    }
  cur_font.clear();

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

    int videoFlags = SDL_GetVideoSurface()->flags;
    SDL_SetVideoMode(xsize+xunused, ysize+yunused, 0, videoFlags);

    glViewport(xunused/2, yunused/2, xsize, ysize);
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

  mWid->Render(cur_time);

  glTranslatef(0.0, 0.0, 0.5);		//Move out in front

  if(popWid) {
    glPushMatrix();
	glTranslatef(popxpos, popypos, 0.0);
    glScalef(popx, popy, 1.0);
    popWid->Render(cur_time);
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
      glTexCoord2f(0.0, mouse_cursor->yfact);
      glVertex3f(-1.0, -1.0, 0.0);
      glTexCoord2f(mouse_cursor->xfact, mouse_cursor->yfact);
      glVertex3f( 1.0, -1.0, 0.0);
      glTexCoord2f(mouse_cursor->xfact, 0.0);
      glVertex3f( 1.0, 1.0, 0.0);
      glTexCoord2f(0.0, 0.0);
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

  SG_Texture::EmptyTrash();
  return 1;
  }

bool SimpleGUI::PollEvent(SDL_Event *event, bool ts) {
  if(!ts) SDL_PumpEvents();	//Only pump once!
  while(SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS) > 0) {
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
  int res = SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS);
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
    res = SDL_PeepEvents(event, 1, SDL_GETEVENT, SDL_ALLEVENTS);
    }
  return false;
  }

bool SimpleGUI::ProcessEvent(SDL_Event *event) {
  if(!event) return 0;

  if(event->type == SDL_VIDEORESIZE) {
    newxunused = 0;
    newyunused = 0;
    newxsize = event->resize.w;
    newysize = event->resize.h;

    float asp = (float)(newxsize) / (float)(newysize);
    if((aspect_method & ASPECT_FIXED_X) && asp > aspect) {
      newxsize = int((float)(newysize)*aspect+0.5);
      newxunused = (event->resize.w - newxsize);
      }
    else if((aspect_method & ASPECT_FIXED_Y) && asp < aspect) {
      newysize = int((float)(newxsize)/aspect+0.5);
      newyunused = (event->resize.h - newysize);
      }
    newaspect_actual = asp / aspect;
    return 0; //Event Handled
    }

  else if(event->type == SDL_KEYDOWN) {
    if(focus_widget) {
      if(mWid->HasWidget(focus_widget)) {
	return mWid->HandEventTo(focus_widget, event, mousex, mousey);
	}
      if(popWid->HasWidget(focus_widget)) {
	return popWid->HandEventTo(focus_widget, event, mousex, mousey);
	}
      focus_widget->HandEventTo(focus_widget, event, 0.0, 0.0);
      }
    }

  else if(event->type == SDL_KEYUP) {
    if(focus_widget) {
      if(mWid->HasWidget(focus_widget)) {
	return mWid->HandEventTo(focus_widget, event, mousex, mousey);
	}
      if(popWid->HasWidget(focus_widget)) {
	return popWid->HandEventTo(focus_widget, event, mousex, mousey);
	}
      focus_widget->HandEventTo(focus_widget, event, 0.0, 0.0);
      }
    }

  if(flags & SIMPLEGUI_NOMOUSE) return 1;	//No Mouse Events!

  if(event->type == SDL_MOUSEBUTTONDOWN) {
    mb_state |= SDL_BUTTON(event->button.button);
    if(current_widget) return 0;		// Ignore another press

    focus_widget = NULL;	// They must reclaim this if it's still theirs

    mousex = float(event->button.x);
    mousey = float(event->button.y);
    ScreenToRelative(mousex, mousey);

    if(popWid && mousex < popx && mousey < popy
	&& mousex > -popx && mousey > -popy) {
      return popWid->HandleEvent(event, mousex/popx, mousey/popy);
      }
    else if(popWid && pop_modal) {
      return 0;	//Eat button events that miss a modal popup
      }

    return mWid->HandleEvent(event, mousex, mousey);
    }

  else if(event->type == SDL_MOUSEBUTTONUP) {
    mb_state &= (~SDL_BUTTON(event->button.button));
    if(current_widget) {
      if(mb_state) return 0;	//Ignore if any button is still pressed

      mousex = float(event->button.x);
      mousey = float(event->button.y);
      ScreenToRelative(mousex, mousey);

      int ret = 0;
      ret = mWid->HandEventTo(current_widget, event, mousex, mousey);
      if(popWid && ret && event->type != SDL_SG_EVENT) {
	ret = popWid->HandEventTo(current_widget, event,
		mousex/popx, mousey/popy);
	}
      if(current_widget && ret && event->type != SDL_SG_EVENT) {
	ret = current_widget->HandleEvent(event, 0.0, 0.0);
	}
      current_widget = NULL; 
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
      if(popWid && ret && event->type != SDL_SG_EVENT) {
	ret = popWid->HandEventTo(current_widget, event,
		mousex/popx, mousey/popy);
	}
      if(current_widget && ret && event->type != SDL_SG_EVENT) {
	ret = current_widget->HandleEvent(event, 0.0, 0.0);
	}
      }

    if(popWid && ret && event->type != SDL_SG_EVENT
	&& mousex < popx && mousey < popy
	&& mousex > -popx && mousey > -popy) {
      ret = popWid->HandleEvent(event, mousex/popx, mousey/popy);
      }

    if(ret && event->type != SDL_SG_EVENT && ((!popWid) || (!pop_modal)))
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
  if(!TTF_WasInit()) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n",
	fontfile, TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);
    }

  if(fontfile) delete [] fontfile;
  fontfile = NULL;
  cur_font[0] = font;
  fontyratio = 1.0;
  default_pxsize = 0;
  }

void SimpleGUI::SetDefaultFontSize(int pxsz) {
  default_pxsize = pxsz;
  }

void SimpleGUI::LoadFont(const char *fontfn, int pxsz) {
  if(fontfile) delete [] fontfile;
  fontfile = new char[strlen(fontfn)+1];
  strcpy(fontfile, fontfn);

  fontyratio = 1.0;
  int load_pxsz = 100;
  Font(load_pxsz); //Initialize font
  int act_height = TTF_FontHeight(cur_font[load_pxsz]);
  if(act_height != load_pxsz) { //Non-true-pixel font
    cur_font[act_height] = cur_font[load_pxsz];
    cur_font.erase(load_pxsz);
    fontyratio = (float)(load_pxsz) / (float)(act_height);
    Font(load_pxsz); //Create the REAL default font size
//    fprintf(stderr, "FontYRatio = %f\n", fontyratio);
    }

  SetDefaultFontSize(pxsz);
  }

TTF_Font *SimpleGUI::Font(int pxsz) {
  if(pxsz < 1) pxsz = default_pxsize;
  if(cur_font.count(pxsz)) return cur_font[pxsz];
  else if(!fontfile) {
//    fprintf(stderr, "WARNING: attempt to resize font size with no loaded font!\n");
    return NULL;
    }

  if(!TTF_WasInit()) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n",
	fontfile, TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);
    }

  int ptsz = (int)((float)(pxsz) * fontyratio + 0.5);	//Scale to real ptsize

  cur_font[pxsz] = TTF_OpenFont(fontfile, ptsz);
  if(!cur_font[pxsz]) {
    fprintf(stderr, "ERROR: Unable to load font '%s'!\n", fontfile);
    exit(1);
    }

  return cur_font[pxsz];
  }

float SimpleGUI::Red(int c) {
  return col[c*2 + 0].r / 255.0f;
  }

float SimpleGUI::Green(int c) {
  return col[c*2 + 0].g / 255.0f;
  }

float SimpleGUI::Blue(int c) {
  return col[c*2 + 0].b / 255.0f;
  }

float SimpleGUI::TextRed(int c) {
  return col[c*2 + 1].r / 255.0f;
  }

float SimpleGUI::TextGreen(int c) {
  return col[c*2 + 1].g / 255.0f;
  }

float SimpleGUI::TextBlue(int c) {
  return col[c*2 + 1].b / 255.0f;
  }

const SDL_Color *SimpleGUI::BGColor(int c) {
  return &col[c*2 + 0];
  }

const SDL_Color *SimpleGUI::TextColor(int c) {
  return &col[c*2 + 1];
  }

void SimpleGUI::SetDefaultTextColor(float tr, float tg, float tb) {
  text_col.r = (Uint8)(tr*255.0f);
  text_col.g = (Uint8)(tg*255.0f);
  text_col.b = (Uint8)(tb*255.0f);
  }

const SDL_Color *SimpleGUI::DefaultTextColor() {
  return &text_col;
  }

void SimpleGUI::SetColor(int c, float r, float g, float b,
	float tr, float tg, float tb) {
  col[c*2 + 0].r = (Uint8)(r*255.0f);
  col[c*2 + 0].g = (Uint8)(g*255.0f);
  col[c*2 + 0].b = (Uint8)(b*255.0f);
  if(r == -1.0) {
    col[c*2 + 1] = text_col;
    }
  else {
    col[c*2 + 1].r = (Uint8)(tr*255.0f);
    col[c*2 + 1].g = (Uint8)(tg*255.0f);
    col[c*2 + 1].b = (Uint8)(tb*255.0f);
    }
  };

int SimpleGUI::NewColor() {
  int ret = col.size();
  col.resize(ret+2);
  return ret/2;
  }

int SimpleGUI::NewColor(float r, float g, float b,
	float tr, float tg, float tb) {
  int ret = NewColor();
  SetColor(ret, r, g, b, tr, tg, tb);
  return ret;
  }

void SimpleGUI::SetPopupWidget(SG_Alignment *wid, float px, float py,
	float posx, float posy) {
  popWid = wid;
  popxpos = posx;
  popypos = posy;
  popx = px;
  popy = py;
  pop_modal = false;
  }
  
void SimpleGUI::SetModalPopupWidget(SG_Alignment *wid, float px, float py,
	float posx, float posy) {
  SetPopupWidget(wid, px, py, posx, posy);
  pop_modal = true;
  }
  
extern int nextpoweroftwo(int);

void SimpleGUI::SetMouseCursor(SDL_Surface *cur, float xsc, float ysc) {
  if(mouse_cursor) delete mouse_cursor;
  mouse_cursor = new SG_Texture(cur);
  mouse_xscale = xsc;
  mouse_yscale = ysc;

  int xsize = nextpoweroftwo(mouse_cursor->src->w);
  int ysize = nextpoweroftwo(mouse_cursor->src->h);
  mouse_cursor->cur = SDL_CreateRGBSurface(0, xsize, ysize, 32,
	SG_SDL_RGBA_COLFIELDS);
  SDL_SetAlpha(mouse_cursor->src, 0, SDL_ALPHA_TRANSPARENT);
  SDL_BlitSurface(mouse_cursor->src, NULL, mouse_cursor->cur, NULL);
  mouse_cursor->xfact = (float)(mouse_cursor->src->w) / (float)(xsize);
  mouse_cursor->yfact = (float)(mouse_cursor->src->h) / (float)(ysize);

  mouse_cursor->Update();
  }

SimpleGUI *SimpleGUI::Current() {
  return current_sg;
  }

SDL_mutex *SimpleGUI::Mutex() {
  if(!mutex) mutex = SDL_CreateMutex();
  return mutex;
  }
