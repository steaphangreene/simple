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

#include <GL/glew.h>
#include "SDL.h"

#define SV_ORTHO	1	// If not present, it's perspective mode

class SimpleVideo {
public:
  SimpleVideo(int xs, int ys, unsigned int flgs, double asp);
  ~SimpleVideo();
  bool StartScene(double zoom = 4.0, double x = 0.0, double y = 0.0);
  bool FinishScene();
  bool ToggleFullscreen();
  bool Resize(int, int);

  static SimpleVideo *CurrentVideo() { return current; }

protected:
  static SimpleVideo *current;

  SDL_Surface *surface;
  int videoFlags;

  int xsize, ysize;
  int hgap, vgap;

  int fullscreen_mode;
  unsigned int flags;

  double aspect;
  };

#endif // SV_H
