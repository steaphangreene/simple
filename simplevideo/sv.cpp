// *************************************************************************
//  This file is part of the SimpleVideo Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleVideo is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleVideo is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleVideo (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SDL_DEBUG		// Just define SDL_DEBUG to add debug
#define SDL_DEBUG	0	// options to SDL_Init()
#endif	//SDL_DEBUG		// such as SDL_NO_PARACHUTE

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifndef M_PI
#define M_PI		3.14159265358979323846  /* pi */
#endif
#define DEG2RAD(d)	((d)*M_PI/180.0)

using namespace std;

#include "sv.h"

SimpleVideo *SimpleVideo::current = NULL;

SimpleVideo::SimpleVideo(int xs, int ys, unsigned int flgs, double asp) {
  if(current) {
    fprintf(stderr, "ERROR: Created mulitple SimpleVideo instances!\n");
    exit(1);
    }
  current = this;

  aspect = asp;
  flags = flgs;

  surface = NULL;
  videoFlags = 0;
  xsize=0; ysize=0;
  hgap=0; vgap=0;
  fullscreen_mode = 0;
  
  const SDL_VideoInfo *videoInfo;
//  GLfloat light1_pos[] = { 10.0, -10.0, 10.0, 0.0 };
//  GLfloat light2_pos[] = { 2.75, 1.5, -3.0, 0.0 };

  xsize = xs;   ysize = ys;

  SDL_GL_LoadLibrary(NULL);	// Load up OpenGL Dynamically
				// (Is this loaded version really being used?)

//  GLenum err = glewInit();	//Don't need this, not using extensions
//
//  if(err != GLEW_OK) {
//    fprintf(stderr, "ERROR: GLEW did not initialize!\n");
//    fprintf(stderr, "ERROR: %s\n", glewGetErrorString(err));
//    exit(0);
//    }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_DEBUG) != 0) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    exit(0);
    }
  atexit(SDL_Quit);

  videoInfo = SDL_GetVideoInfo();

  videoFlags = SDL_OPENGL;
  videoFlags |= SDL_GL_DOUBLEBUFFER;
  videoFlags |= SDL_RESIZABLE;
  if(fullscreen_mode)
    videoFlags |= SDL_FULLSCREEN;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  surface = SDL_SetVideoMode(xsize, ysize, 0, videoFlags);

  if(surface == NULL) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    exit(0);
    }

  // Set a window title.
  SDL_WM_SetCaption("SimpleVideo Renderer", "SimpleVideo Renderer");

  // Set the clear color to black
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClearDepth(1.0);

  // Set the shading model
  glShadeModel(GL_SMOOTH);
//  glShadeModel(GL_FLAT);

  // Set the polygon mode to fill
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

// Define how to determine if something's below something else
  glDepthFunc(GL_LEQUAL);
//  glDepthFunc(GL_LESS);

  // Enable depth testing for hidden line removal
  glEnable(GL_DEPTH_TEST);

  // Hopefully you don't need this - slows things down quite a bit
//  glEnable(GL_NORMALIZE);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  //Other settings to tinker with
//  glEnable (GL_POLYGON_SMOOTH);
//  if(glError());
//  glHint(GL_POLYGON_SMOOTH,GL_FASTEST);

//  glEnable(GL_POINT_SMOOTH);
//  glEnable(GL_LINE_SMOOTH);

  // Prepare for (but don't currently enable) alpha blending
  glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);

  // Set the GL_AMBIENT_AND_DIFFUSE color state variable to be the
  // one referred to by all following calls to glColor
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  // Create a Directional Light Source
//  glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
//  glLightfv(GL_LIGHT1, GL_POSITION, light2_pos);
//  glEnable(GL_LIGHT0);
//  glEnable(GL_LIGHTING);

  // Set the new viewport size
  glViewport(0, 0, (GLint)xsize, (GLint)ysize);

  // Enable 2D textures by default
  glEnable(GL_TEXTURE_2D);

  xp = 0.0;
  yp = 0.0;
  targ_xp = xp;
  targ_yp = yp;
  pos_start = 0;
  pos_delay = 0;

  dxp = 0.0;
  dyp = 0.0;
  move_start = 0;

  angle = 0.0;
  targ_angle = angle;
  angle_start = 0;
  angle_delay = 0;

  down = 60.0;
  if(flags & SV_ORTHO) down = 30.0;
  targ_down = down;
  down_start = 0;
  down_delay = 0;

  zoom = 4.0;
  targ_zoom = zoom;
  zoom_start = 0;
  zoom_delay = 0;
  }

SimpleVideo::~SimpleVideo() {
  //at_exit handles all this
  }

bool SimpleVideo::StartScene() {
  Uint32 real_time = SDL_GetTicks();

  double x = 0.0, y = 0.0, ang = 0.0, xoff = 0.0, yoff = 0.0;
  CalcMove(xoff, yoff, real_time);
  CalcPos(x, y, real_time);
  CalcAng(ang, real_time);

//Catch up when move is over
  if(real_time >= pos_start + pos_delay) {
    xp = x;
    yp = y;
    }

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  //This is the actual perspective setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(flags & SV_ORTHO) {
    glOrtho(-16.0/9.0*zoom*4, 16.0/9.0*zoom*4,
	-1.0*zoom*4, 1.0*zoom*4,
	1.0, 64.0);
    }
  else {
    gluPerspective(45.0, 16.0/9.0, 1.0, 64.0);
    }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  double vdist = 8.0;
  if(flags & SV_ORTHO) {
    double xvp, yvp, zvp;
    zvp = vdist * sin(DEG2RAD(down));
    xvp = vdist * cos(DEG2RAD(down)) * sin(DEG2RAD(ang));
    yvp = vdist * cos(DEG2RAD(down)) * -cos(DEG2RAD(ang));

    double normx = 0.0, normy = 0.0, normz = 1.0;
    if(xvp == 0.0 && yvp == 0.0) normy = 1.0;	//Make sure we know where's up.

    gluLookAt(xvp+x+xoff, yvp+y+yoff, zvp, x+xoff, y+yoff, 0.0, normx, normy, normz);
    }
  else {
    gluLookAt(zoom*4+x, zoom*4+y, zoom*4*2, x, y, 0.0, -zoom, -zoom, 0.0);
    }
  return true;
  }

bool SimpleVideo::FinishScene() {
  glFlush();
  SDL_GL_SwapBuffers();

  return true;
  }

bool SimpleVideo::Resize(int xs, int ys) {
  int rx = xs, ry = ys;
  surface = SDL_SetVideoMode(rx, ry, 0, videoFlags);

  xsize = xs;   ysize = ys;

  if(xsize > (ysize*4)/3) xsize = (ysize*4)/3;
  if(ysize > (xsize*3)/4) ysize = (xsize*3)/4;

  hgap = (rx-xsize)/2;  vgap = (ry-ysize)/2;
  glViewport(hgap, vgap, (GLint)xsize, (GLint)ysize);

  return true;
  }

static int oldmodex = 0, oldmodey = 0;	//Temporary!

bool SimpleVideo::ToggleFullscreen(void) {
  char drv[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  SDL_VideoDriverName((char *)drv, 15);
  if(strcmp(drv, "x11")) return false;

  if(oldmodex != 0) {
    Resize(oldmodex, oldmodey);
    oldmodex = 0;  oldmodey = 0;
    }
  else {
    int ctr, goal=-1;
    SDL_Rect **modes;

    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

    if(modes == (SDL_Rect **)0){
      modes = SDL_ListModes(NULL, SDL_FULLSCREEN);
      }
    if(modes == (SDL_Rect **)0){
      return false;
      }

    oldmodex = surface->w;
    oldmodey = surface->h;

    for(ctr=0; modes[ctr]; ++ctr) {
      if(modes[ctr]->w >= surface->w && modes[ctr]->h >= surface->h) {
	if(goal == -1
		|| modes[ctr]->w < modes[goal]->w
		|| modes[ctr]->h < modes[goal]->h) {
	  goal = ctr;
	  }
	}
      }
    if(goal == -1) {
      goal = 0;
      for(ctr=1; modes[ctr]; ++ctr) {
	if(modes[ctr]->w > modes[goal]->w || modes[ctr]->h > modes[goal]->h) {
	  goal = ctr;
	  }
	}
      }
    Resize(modes[goal]->w, modes[goal]->h);
    }
  SDL_WM_ToggleFullScreen(surface);
  videoFlags ^= SDL_FULLSCREEN;
  fullscreen_mode = (videoFlags & SDL_FULLSCREEN) != 0;

  return true;
  }

void SimpleVideo::SetPosition(double x, double y, Uint32 delay) {
  Uint32 event_time = SDL_GetTicks();

  //Start from where we are right now
  double tmpx = 0.0, tmpy = 0.0;
  CalcPos(tmpx, tmpy, event_time);
  xp = tmpx;
  yp = tmpy;

  targ_xp = x;
  targ_yp = y;
  pos_start = event_time;
  pos_delay = delay;
  }

void SimpleVideo::CalcPos(double &x, double &y, Uint32 cur_time) {
  x = xp;
  y = yp;

  if(targ_xp != xp || targ_yp != yp) {
    if(cur_time >= pos_start + pos_delay) {
      x = targ_xp;
      y = targ_yp;
      }
    else if(cur_time > pos_start) {
      //This would be linearly interpolated movement
      //Uint32 part = cur_time - pos_start;
      //Uint32 ipart = pos_delay - part;
      //x = (xp*ipart + targ_xp*part) / pos_delay;
      //y = (yp*ipart + targ_yp*part) / pos_delay;

      //This is smoother
      double frac = (double)(cur_time - pos_start) / (double)(pos_delay);
      double part = sin(frac * M_PI/2.0);
      part = part;
      double ipart = 1.0 - part;
      x = (xp*ipart + targ_xp*part);
      y = (yp*ipart + targ_yp*part);
      }
    }
  }

void SimpleVideo::SetMove(double dx, double dy) {
  Uint32 cur_time = SDL_GetTicks();

  double xoff, yoff;
  CalcMove(xoff, yoff, cur_time);

  xp += xoff;
  yp += yoff;
  targ_xp += xoff;
  targ_yp += yoff;

  dxp = dx;
  dyp = dy;
  move_start = cur_time;
  }

void SimpleVideo::CalcMove(double &xoff, double &yoff, Uint32 cur_time) {
  double elapsed = cur_time - move_start;

  xoff = (dxp * 2.0 * elapsed / 1000.0) * cos(DEG2RAD(targ_angle));
  xoff += (dyp * 2.0 * elapsed / 1000.0) * -sin(DEG2RAD(targ_angle));

  yoff = (dxp * 2.0 * elapsed / 1000.0) * sin(DEG2RAD(targ_angle));
  yoff += (dyp * 2.0 * elapsed / 1000.0) * cos(DEG2RAD(targ_angle));
  }

void SimpleVideo::SetZoom(double zm, Uint32 delay) {
  //FIXME: Smooth Interpolation!
  zoom = zm;
  }

void SimpleVideo::SetAngle(double ang, Uint32 delay) {
  SetMove(dxp, dyp); //This will currentize the movement

  Uint32 event_time = SDL_GetTicks();

  //Start from where we are right now
  double tmpang = 0.0;
  CalcAng(tmpang, event_time);
  angle = tmpang;

  targ_angle = ang;
  angle_start = event_time;
  angle_delay = delay;
  }

void SimpleVideo::CalcAng(double &ang, Uint32 cur_time) {
  ang = angle;

  if(targ_angle != angle) {
    if(cur_time >= angle_start + angle_delay) {
      ang = targ_angle;
      }
    else if(cur_time > angle_start) {
      double frac = (double)(cur_time - angle_start) / (double)(angle_delay);
      double part = sin(frac * M_PI/2.0);
      part = part;
      double ipart = 1.0 - part;
      ang = (angle*ipart + targ_angle*part);
      }
    }
  }

void SimpleVideo::SetDown(double dn, Uint32 delay) {
  //FIXME: Smooth Interpolation!
  down = dn;
  }
