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

#include "sv_perspective.h"

SV_Perspective::SV_Perspective(int xs, int ys) : SimpleVideo(xs, ys) {
  }

bool SV_Perspective::StartScene(double zoom, double x, double y) {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  //This is the actual perspective setup
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 16.0/9.0, 1.0, 64.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(zoom*4+x, zoom*4+y, zoom*4*2, x, y, 0.0, -zoom, -zoom, 0.0);
  return true;
  }

SV_Perspective *SV_Perspective::current = NULL;
