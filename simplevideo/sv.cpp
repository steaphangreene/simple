// *************************************************************************
//  This file is part of the SimpleVideo Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

#define MIN_DOWN	5.0	//Minimum of 5 degrees angle to z plane

using namespace std;

#include "sv.h"

SimpleVideo *SimpleVideo::current = NULL;

SimpleVideo::SimpleVideo(int xs, int ys, double asp, bool fullscr) {
  if(current) {
    fprintf(stderr, "ERROR: Created mulitple SimpleVideo instances!\n");
    exit(1);
    }
  current = this;

  aspect = asp;
  flags = 0;
  yfov = 45.0;

  surface = NULL;
  videoFlags = 0;
  xsize=0; ysize=0;
  hgap=0; vgap=0;
  fullscreen_mode = fullscr;
  
  const SDL_VideoInfo *videoInfo;
//  GLfloat light1_pos[] = { 10.0, -10.0, 10.0, 0.0 };
//  GLfloat light2_pos[] = { 2.75, 1.5, -3.0, 0.0 };

  xsize = xs;   ysize = ys;

  xstart = -1.0;
  xend = 1.0;
  ystart = -1.0;
  yend = 1.0;

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

  minz = 0.0, maxz = 10.0;

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

void SimpleVideo::SetOrtho() {
  flags = SV_ORTHO;
  }

void SimpleVideo::SetPerspective(double vert_fov) {
  if(vert_fov > 90.0) vert_fov = 90.0;
  else if(vert_fov < 0.0) vert_fov = 0.0;

  flags = 0;
  yfov = vert_fov;

  if(down < MIN_DOWN + (yfov/2.0)) down = MIN_DOWN + (yfov/2.0);
  if(targ_down < MIN_DOWN + (yfov/2.0)) targ_down = MIN_DOWN + (yfov/2.0);
  }

SimpleVideo::~SimpleVideo() {
  //at_exit handles all this
  }

bool SimpleVideo::StartScene() {
  Uint32 real_time = SDL_GetTicks();

  double x = 0.0, y = 0.0, ang = 0.0, xoff = 0.0, yoff = 0.0, zm = 0.0;
  CalcMove(xoff, yoff, real_time);
  CalcZoom(zm, real_time);
  CalcPos(x, y, real_time);
  CalcAng(ang, real_time);

  //Catch up with transitions that are over
  if((xp != targ_xp || yp != targ_yp) && real_time >= pos_start + pos_delay) {
    xp = targ_xp;
    yp = targ_yp;
    }
  if(angle != targ_angle && real_time >= angle_start + angle_delay) {
    angle = targ_angle;
    }
  if(down != targ_down && real_time >= down_start + down_delay) {
    down = targ_down;
    }
  if(zoom != targ_zoom && real_time >= zoom_start + zoom_delay) {
    zoom = targ_zoom;
    }

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Enable depth testing for hidden line removal
  glEnable(GL_DEPTH_TEST);

  // Enable 2D textures by default
  glEnable(GL_TEXTURE_2D);

  //This is the actual perspective setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glTranslatef((xend+xstart)/2.0, (yend+ystart)/2.0, 0.0);
  glScalef((xend-xstart)/2.0, (yend-ystart)/2.0, (yend-ystart)/2.0);

  if(flags & SV_ORTHO) {
    //FIXME: Calculate ACTUAL near and far clipping limits
    glOrtho(-aspect*zm/2.0, aspect*zm/2.0, -1.0*zm/2.0, 1.0*zm/2.0, 1.0, 64.0);
    }
  else {
    //FIXME: Calculate ACTUAL near and far clipping limits
    gluPerspective(yfov, aspect, 1.0, 64.0);
    }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  double vdist = 32.0;
  if(!(flags & SV_ORTHO)) vdist = zm;

  double xvp, yvp, zvp;
  zvp = vdist * sin(DEG2RAD(down));
  xvp = vdist * cos(DEG2RAD(down)) * sin(DEG2RAD(ang));
  yvp = vdist * cos(DEG2RAD(down)) * -cos(DEG2RAD(ang));

  double upx = sin(DEG2RAD(down)) * -sin(DEG2RAD(ang));
  double upy = sin(DEG2RAD(down)) * cos(DEG2RAD(ang));
  double upz = cos(DEG2RAD(down));

  gluLookAt(xvp+x+xoff, yvp+y+yoff, zvp, x+xoff, y+yoff, 0.0, upx, upy, upz);

  glGetDoublev(GL_MODELVIEW_MATRIX, modelv);
  glGetDoublev(GL_PROJECTION_MATRIX, projv);
  glGetIntegerv(GL_VIEWPORT, viewport);

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
  Uint32 event_time = SDL_GetTicks();

  //Start from where we are right now
  double tmpzm = 0.0;
  CalcZoom(tmpzm, event_time);
  zoom = tmpzm;

  targ_zoom = zm;
  zoom_start = event_time;
  zoom_delay = delay;
  }

void SimpleVideo::CalcZoom(double &zm, Uint32 cur_time) {
  zm = zoom;

  if(targ_zoom != zoom) {
    if(cur_time >= zoom_start + zoom_delay) {
      zm = targ_zoom;
      }
    else if(cur_time > zoom_start) {
      double frac = (double)(cur_time - zoom_start) / (double)(zoom_delay);
      double part = sin(frac * M_PI/2.0);
      part = part;
      double ipart = 1.0 - part;
      zm = (zoom*ipart + targ_zoom*part);
      }
    }
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
  if(dn > 90.0) dn = 90.0;
  else if(dn < MIN_DOWN) dn = MIN_DOWN;

  down = dn;
  targ_down = down;

  //Make sure this setting is acceptable for Perspective view
  if(!(flags & SV_ORTHO)) SetPerspective(yfov);
  }

void SimpleVideo::SetZExtents(double mnz, double mxz) {
  minz = mnz;
  maxz = mxz;
  }

void SimpleVideo::ScreenToMap(double &x, double &y, const double &z) {
  GLint fake_viewport[4] = { 0, 0, 2, 2 };

  x = x + 1.0;
  y = y + 1.0;

  x *= (xend-xstart)/2.0;	//Handle Subscreen Translation
  y *= (yend-ystart)/2.0;
  x += (xstart+1.0);
  y += (ystart+1.0);

  GLdouble x0, x1, y0, y1, z0, z1;
  if(!gluUnProject(x, y, 0.0, modelv, projv, fake_viewport, &x0, &y0, &z0)) {
    fprintf(stderr, "WARNING: gluUnProject Failed!\n");
    }
  if(!gluUnProject(x, y, 1.0, modelv, projv, fake_viewport, &x1, &y1, &z1)) {
    fprintf(stderr, "WARNING: gluUnProject Failed!\n");
    }

  x1 = x1 - x0;		// x1 is now dX for view_z:1.0
  y1 = y1 - y0;		// y1 is now dY for view_z:1.0
  z1 = z1 - z0;		// a1 is now dZ for view_z:1.0

  if(z1 == 0.0) {	//Don't actually cross Z plane
    x = 0.0;
    y = 0.0;
    return;
    }

  GLdouble fact = (z - z0) / z1;
  x = x0 + x1*fact;
  y = y0 + y1*fact;
  }

void SimpleVideo::MapToScreen(double &x, double &y, const double &z) {
  GLint fake_viewport[4] = { 0, 0, 2, 2 };

  GLdouble sx, sy, sz;
  gluProject(x, y, z, modelv, projv, fake_viewport, &sx, &sy, &sz);
  x = sx; y = sy;

  x -= (xstart+1.0);		//Handle Subscreen Translation
  y -= (ystart+1.0);
  x /= (xend-xstart)/2.0;
  y /= (yend-ystart)/2.0;

  x -= 1.0;
  y -= 1.0;
  }

void SimpleVideo::SetSubscreen(double xs, double ys, double xe, double ye) {
  if(xs >= xe || ys >= ye || xs < -1.0 || ys < -1.0 || xe > 1.0 || ye > 1.0) {
    fprintf(stderr, "ERROR: Bad SubScreen values passed to SimpleVideo\n");
    fprintf(stderr, "ERROR: Values (%f,%f) - (%f,%f)\n", xs, ys, xe, ye);
    exit(1);
    }
  xstart = xs;
  xend = xe;
  ystart = ys;
  yend = ye;
  }

void SimpleVideo::ResetSubscreen() {
  SetSubscreen(-1.0, -1.0, 1.0, 1.0);
  }
