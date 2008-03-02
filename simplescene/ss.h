// *************************************************************************
//  This file is part of the SimpleScene Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleScene is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleScene is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleScene (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SS_H
#define SS_H

#include "SDL.h"
#include "SDL_opengl.h"

#include <vector>
using namespace std;

#include "st.h"

struct SimpleScene_ParticleType {
  SimpleTexture *tex;
  float cr0, cg0, cb0, ca0;
  float cr1, cg1, cb1, ca1;
  float xv, yv, zv;
  float sz0, sz1;
  Uint32 dur;
  };

struct SimpleScene_Particle {
  unsigned int type;
  float xp, yp, zp;
  Uint32 start;
  };

class SimpleScene {
public:
  SimpleScene();
  ~SimpleScene();

  bool Render(Uint32 offset);

  static SimpleScene *Current() { return current; };

  int AddParticleType(SimpleScene_ParticleType ptp);
  void AddParticle(int type, float xp, float yp, float zp, Uint32 start);

protected:
  bool DrawParticles(Uint32 offset);

  vector<SimpleScene_ParticleType> ptypes;
  vector<SimpleScene_Particle> parts;

  static SimpleScene *current;
  };

#endif // SS_H
