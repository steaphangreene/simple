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

#ifndef SV_H
#define SV_H

#include "SDL.h"
#include "SDL_opengl.h"

#define SV_ORTHO	1	// If not present, it's perspective mode

class SimpleVideo {
public:
  SimpleVideo(int xs, int ys, double asp);
  ~SimpleVideo();

  void SetOrtho();
  void SetPerspective(double vert_fov);
  void SetSubscreen(double xs, double ys, double xe, double ye);
  void ResetSubscreen();

  bool StartScene();
  bool FinishScene();

  bool ToggleFullscreen();
  bool Resize(int, int);

  void SetMove(double dx, double dy);
  void SetPosition(double x, double y, Uint32 delay);
  void SetZoom(double zm, Uint32 delay);
  void SetAngle(double ang, Uint32 delay);
  void SetDown(double dn, Uint32 delay);

  void SetZExtents(double mnz, double mxz);

  void ScreenToMap(double &x, double &y, const double &z = 0.0);
  void MapToScreen(double &x, double &y, const double &z = 0.0);

  static SimpleVideo *Current() { return current; };
  static SimpleVideo *CurrentVideo() { return Current(); };	// Depricated

protected:
  void CalcMove(double &xoff, double &yoff, Uint32 cur_time);
  void CalcZoom(double &zm, Uint32 cur_time);
  void CalcPos(double &x, double &y, Uint32 cur_time);
  void CalcAng(double &ang, Uint32 cur_time);
  static SimpleVideo *current;

  SDL_Surface *surface;
  int videoFlags;

  int xsize, ysize;
  int hgap, vgap;

  int fullscreen_mode;
  unsigned int flags;

  double aspect;
  double yfov; //Used only by perspective

  double minz, maxz;

  double xp, yp, targ_xp, targ_yp;
  Uint32 pos_start, pos_delay;
  double dxp, dyp;
  Uint32 move_start;

  double angle, targ_angle;
  Uint32 angle_start, angle_delay;

  double zoom, targ_zoom;
  Uint32 zoom_start, zoom_delay;

  double down, targ_down;
  Uint32 down_start, down_delay;

  GLdouble projv[16], modelv[16];
  GLint viewport[4];

  double xstart, xend, ystart, yend;
  };

#endif // SV_H
