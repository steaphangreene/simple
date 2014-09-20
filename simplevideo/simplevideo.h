// *************************************************************************
//  This file is part of the SimpleVideo Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleVideo is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleVideo is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleVideo (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef SIMPLEVIDEO_H
#define SIMPLEVIDEO_H

#include "SDL.h"
#include "SDL_opengl.h"

class SimpleScene;
class SimpleGUI;

#define SV_ORTHO	1	// If not present, it's perspective mode

#include <vector>
using namespace std;

struct SimpleVideo_Mode {
  Uint32 x, y;
  };

class SimpleVideo {
public:
  SimpleVideo(int xs, int ys, float asp, bool fullscr = false);
  ~SimpleVideo();

  void SetOrtho();
  void SetPerspective(float vert_fov);
  void SetSubscreen(float xs, float ys, float xe, float ye);
  void ResetSubscreen();

  void EnableLighting();
  void DisableLighting();

  bool Render(Uint32 cur_time);

  bool StartScene();
  bool FinishScene();

  bool ToggleFullscreen();

  void SetMove(float dx, float dy);
  void SetPosition(float x, float y, Uint32 delay);
  void SetZPosition(float z, Uint32 delay);
  void SetZoom(float zm, Uint32 delay);
  void SetAngle(float ang, Uint32 delay);
  void SetDown(float dn, Uint32 delay);

  void SetZExtents(float mnz, float mxz);

  void ScreenToMap(float &x, float &y, const float &z = 0.0);
  void MapToScreen(float &x, float &y, const float &z = 0.0);

  const vector<SimpleVideo_Mode> GetFullScreenModes() const;
  void SetFullScreenMode(int xs, int ys);
  void SetWindowedMode(int xs, int ys);

  void GetViewLimits(
	float &x0, float &y0, float &z0, float &x1, float &y1, float &z1);

  static SimpleVideo *Current() { return current; };
  static SimpleVideo *CurrentVideo() { return Current(); };	// Depricated

  void SBSOn() { sbs = true; ResizeGL(xsize, ysize); }
  void SBSOff() { sbs = false; ResizeGL(xsize, ysize); }
  void ToggleSBS() { sbs = !sbs; ResizeGL(xsize, ysize); }

  void SetScene(SimpleScene *scene);
  void SetGUI(SimpleGUI *gui);

protected:
  bool ResizeGL(int, int);

  SimpleScene *scene;
  SimpleGUI *gui;

  void CalcMove(float &xoff, float &yoff, Uint32 cur_time);
  void CalcZoom(float &zm, Uint32 cur_time);
  void CalcPos(float &x, float &y, Uint32 cur_time);
  void CalcZPos(float &z, Uint32 cur_time);
  void CalcAng(float &ang, Uint32 cur_time);
  static SimpleVideo *current;

  SDL_Window *window;
  int window_flags;
  SDL_GLContext glcontext;

  int xsize, ysize;
  int hgap, vgap;

  bool fullscreen_mode;
  unsigned int flags;

  float aspect;
  float yfov; //Used only by perspective
  bool sbs; // Side-by-Side 3D

  float minz, maxz;

  float xp, yp, targ_xp, targ_yp;
  Uint32 pos_start, pos_delay;
  float zp, targ_zp;
  Uint32 zpos_start, zpos_delay;
  float dxp, dyp;
  Uint32 move_start;

  float angle, targ_angle;
  Uint32 angle_start, angle_delay;

  float zoom, targ_zoom;
  Uint32 zoom_start, zoom_delay;

  float down, targ_down;
  Uint32 down_start, down_delay;

  GLdouble projv[16], modelv[16];

  float xstart, xend, ystart, yend;
  };

#endif // SIMPLEVIDEO_H
