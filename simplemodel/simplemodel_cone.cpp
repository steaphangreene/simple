// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL.h"

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel_cone.h"

SimpleModel_Cone::SimpleModel_Cone(const string &filenm) {
  }

SimpleModel_Cone::SimpleModel_Cone() {
  }

SimpleModel_Cone::~SimpleModel_Cone() {
  }

bool SimpleModel_Cone::Load(const string &filenm) {
  return true;
  }

bool SimpleModel_Cone::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, true);
  gluCylinder(quad, 1.0, 0.0, 1.0, 16, 8);
  gluDeleteQuadric(quad);
  return true;
  }
