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
  }

SimpleGUI::~SimpleGUI() {
  delete mWid;
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

    if(mWid) return mWid->HandleMouseEvent(event, mousex, mousey);
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
    else if(mWid)
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
