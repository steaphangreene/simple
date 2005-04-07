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

using namespace std;

#include "sv.h"

SimpleVideo::SimpleVideo(int xs, int ys) {
  surface = NULL;
  videoFlags = 0;
  xsize=0; ysize=0;
  hgap=0; vgap=0;
  fullscreen_mode = 0;
  
  const SDL_VideoInfo *videoInfo;
  GLfloat light1_pos[] = { 10.0, -10.0, 10.0, 0.0 };
  GLfloat light2_pos[] = { 2.75, 1.5, -3.0, 0.0 };

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
  videoFlags |= SDL_HWPALETTE;
  videoFlags |= SDL_RESIZABLE;
  if(fullscreen_mode)
    videoFlags |= SDL_FULLSCREEN;

  // Use HW Surfaces if possible
  if(videoInfo->hw_available) videoFlags |= SDL_HWSURFACE;
  else videoFlags |= SDL_SWSURFACE;

  // Use HW Blits if possible
  if(videoInfo->blit_hw) videoFlags|=SDL_HWACCEL;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  surface = SDL_SetVideoMode(xsize, ysize, 0, videoFlags);

  if(surface == NULL) {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    exit(0);
    }

  // Set a window title.
  SDL_WM_SetCaption("Example Renderer", "Example Renderer");

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
//  glEnable(GL_NORMALIZE);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  //Other settings to tinker with
//  glEnable (GL_POLYGON_SMOOTH);
//  if(glError());
//  glHint(GL_POLYGON_SMOOTH,GL_FASTEST);

//  glEnable(GL_POINT_SMOOTH);
//  glEnable(GL_LINE_SMOOTH);


  glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_BLEND);

//  glBlendFunc(GL_ONE, GL_ZERO);
//  glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
//  glBlendFunc(GL_ONE, GL_ONE);
//  glEnable(GL_BLEND);

//  glEnable(GL_BLEND);
//  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  // Set the GL_AMBIENT_AND_DIFFUSE color state variable to be the
  // one referred to by all following calls to glColor
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  // Create a Directional Light Source
  glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
  glLightfv(GL_LIGHT1, GL_POSITION, light2_pos);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  // Set the new viewport size
  glViewport(0, 0, (GLint)xsize, (GLint)ysize);

  glEnable(GL_TEXTURE_2D);
  }

SimpleVideo::~SimpleVideo() {
  //at_exit handles all this
  }

bool SimpleVideo::StartScene(double zoom, double x, double y) {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  //This is the actual perspective setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
//  glFrustum(-0.5, -0.5+((GLdouble)xsize)/((GLdouble)ysize), -0.5, 0.5, 1.5, 6.0);
//  glFrustum(-0.8, 0.8, 0.6, -0.6, 5.0, 20.0);
//  glTranslatef(0.0, 0.0, -4.0);
//  glFrustum(1.0, -1.0, -1.0, 1.0, 1.0, 8.0);
  gluPerspective(45.0, 16.0/9.0, 1.0, 64.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(zoom*4+x, zoom*4+y, zoom*4*2, x, y, 0.0, -zoom, -zoom, 0.0);
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
