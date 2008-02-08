// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel_sphere.h"

SimpleModel_Sphere::SimpleModel_Sphere(const string &filenm) {
  }

SimpleModel_Sphere::SimpleModel_Sphere() {
  }

SimpleModel_Sphere::~SimpleModel_Sphere() {
  }

bool SimpleModel_Sphere::Load(const string &filenm) {
  return true;
  }

bool SimpleModel_Sphere::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  GLUquadric *quad = gluNewQuadric();
  gluQuadricTexture(quad, true);
  gluSphere(quad, 1.0, 16, 16);
  gluDeleteQuadric(quad);
  return true;
  }
