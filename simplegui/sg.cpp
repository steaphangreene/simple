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

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

using namespace std;

#include "simplegui.h"
#include "sg_alignment.h"

SimpleGUI *current_sg = NULL;

SimpleGUI::SimpleGUI(int aspmeth, float asp) {
  mWid = new SG_Alignment();
  aspect = asp;
  aspect_actual = 1.0;
  aspect_method = aspmeth;

  int screen_geom[4];
  glGetIntegerv(GL_VIEWPORT, screen_geom);
  xoffset = screen_geom[0];
  yoffset = screen_geom[1];
  xsize = screen_geom[2];
  ysize = screen_geom[3];

  SDL_ShowCursor(0);

  mb_state = 0;

  mousex = -100.0;	// So system knows not to draw cursor until it moves
  mousey = -100.0;

  current_sg = this;
  current_widget = NULL;

  col.resize(SG_COL_MAX);

  col[SG_COL_BG].r = 60;
  col[SG_COL_BG].g = 60;
  col[SG_COL_BG].b = 60;

  col[SG_COL_FG].r = 90;
  col[SG_COL_FG].g = 90;
  col[SG_COL_FG].b = 90;

  col[SG_COL_RAISED].r = 150;
  col[SG_COL_RAISED].g = 150;
  col[SG_COL_RAISED].b = 150;

  col[SG_COL_LOW].r = 30;
  col[SG_COL_LOW].g = 30;
  col[SG_COL_LOW].b = 30;

  col[SG_COL_HIGH].r = 90;
  col[SG_COL_HIGH].g = 90;
  col[SG_COL_HIGH].b = 150;

  col[SG_COL_TEXT].r = 0;
  col[SG_COL_TEXT].g = 0;
  col[SG_COL_TEXT].b = 0;

  cur_font = NULL;
  }

SimpleGUI::~SimpleGUI() {
  delete mWid;
  mWid = NULL;

  if(cur_font) TTF_CloseFont(cur_font);
  cur_font = NULL;
  }

bool SimpleGUI::Render(unsigned long cur_time) {
  //This basically leverages the perspective to flat 2D
  //with a -1.0 to 1.0 centered coord system
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  if(aspect_actual > 1.0 && (aspect_method & ASPECT_EXPANDING_X)) {
    glScalef(1.0/aspect_actual, 1.0, 1.0);
    }
  else if(aspect_actual < 1.0 && (aspect_method & ASPECT_EXPANDING_Y)) {
    glScalef(1.0, aspect_actual, 1.0);
    }

  mWid->Render();

  //Now we draw the mouse cursor (if at least one axis is within coord system)
  if((mousex >= -1.0 && mousex <= 1.0) || (mousey >= -1.0 && mousey <= 1.0)) {
    glTranslatef(mousex, mousey, 0.5);		//Just a square cursor for now
    glScalef(0.0625/aspect, 0.0625, 1.0);

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

  //This puts the perspective back where it was
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();   
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  return 1;
  }

bool SimpleGUI::ProcessEvent(SDL_Event *event) {
  if(!event) return 0;

  if(event->type == SDL_VIDEORESIZE) {
    xoffset = 0;
    yoffset = 0;
    xsize = event->resize.w;
    ysize = event->resize.h;

    int videoFlags = SDL_GetVideoSurface()->flags;
    SDL_SetVideoMode(event->resize.w, event->resize.h, 0, videoFlags);

    float asp = (float)(xsize) / (float)(ysize);
    if((aspect_method & ASPECT_FIXED_X) && asp > aspect) {
      xsize = int((float)(ysize)*aspect+0.5);
      xoffset = (event->resize.w - xsize) / 2;
      }
    else if((aspect_method & ASPECT_FIXED_Y) && asp < aspect) {
      ysize = int((float)(xsize)/aspect+0.5);
      yoffset = (event->resize.h - ysize) / 2;
      }

    aspect_actual = asp / aspect;
    glViewport(xoffset, yoffset, xsize, ysize);

    return 0; //Event Handled
    }

  else if(event->type == SDL_MOUSEBUTTONDOWN) {
    mb_state |= SDL_BUTTON(event->button.button);
    if(current_widget) return 0;		// Ignore another press

    mousex = float(event->button.x);
    mousey = float(event->button.y);
    ScreenToRelative(mousex, mousey);

    return mWid->HandleMouseEvent(event, mousex, mousey);
    }

  else if(event->type == SDL_MOUSEBUTTONUP) {
    mb_state &= (~SDL_BUTTON(event->button.button));
    if(current_widget) {
      if(mb_state) return 0;	//Ignore if any button is still pressed

      mousex = float(event->button.x);
      mousey = float(event->button.y);
      ScreenToRelative(mousex, mousey);

      return mWid->HandMouseEventTo(current_widget, event, mousex, mousey);
      }
    }

  else if(event->type == SDL_MOUSEMOTION) {
    mousex = float(event->motion.x);
    mousey = float(event->motion.y);
    ScreenToRelative(mousex, mousey);

    if(current_widget)
      return mWid->HandMouseEventTo(current_widget, event, mousex, mousey);
    else
      return mWid->HandleMouseEvent(event, mousex, mousey);
    }

  return 1;
  }

int SimpleGUI::ScreenToRelative(float &x, float &y) {
//  fprintf(stderr, "(%f,%f) on (%d,%d):%dx%d\n", x, y,
//	xoffset, yoffset, xsize, ysize);

  if(xsize <= 0 || ysize <= 0 || xoffset < 0 || yoffset < 0) { //Invalid
    return 0;
    }

  x -= (float)(xoffset);
  y -= (float)(yoffset);

  if(x < 0.0 || y < 0.0 || x >= (float)(xsize) || y >= (float)(ysize)) {
    return 0;
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

  return 1;
  }

void SimpleGUI::LoadFont(const char *fontfn) {
  if(!cur_font) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n",
	fontfn, TTF_GetError());
      exit(1);
      }
    atexit(TTF_Quit);

    cur_font = TTF_OpenFont(fontfn, 100);
    if(!cur_font) {
      fprintf(stderr, "ERROR: Unable to load font '%s'!\n", fontfn);
      exit(1);
      }
    }
  }

float SimpleGUI::Red(int c) {
  return col[c].r / 255.0f;
  }

float SimpleGUI::Green(int c) {
  return col[c].g / 255.0f;
  }

float SimpleGUI::Blue(int c) {
  return col[c].b / 255.0f;
  }

const SDL_Color *SimpleGUI::Color(int c) {
  return &col[c];
  }

void SimpleGUI::SetColor(int c, float r, float g, float b) {
  col[c].r = (Uint8)(r*255.0f);
  col[c].g = (Uint8)(g*255.0f);
  col[c].b = (Uint8)(b*255.0f);
  };

int SimpleGUI::NewColor() {
  int ret = col.size();
  col.resize(ret+1);
  return ret;
  }

int SimpleGUI::NewColor(float r, float g, float b) {
  int ret = NewColor();
  SetColor(ret, r, g, b);
  return ret;
  }
