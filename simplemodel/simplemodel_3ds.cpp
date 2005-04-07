// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
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
#include <GL/glew.h>

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel_3ds.h"

SimpleModel_3DS::SimpleModel_3DS(const string &filenm) {
  //Fixme: Unimplemented!
  }

SimpleModel_3DS::SimpleModel_3DS() {
  }

SimpleModel_3DS::~SimpleModel_3DS() {
  }

bool SimpleModel_3DS::Load(const string &filenm) {
  //Fixme: Unimplemented!
  return true;
  }

bool SimpleModel_3DS::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const {
  //Fixme: Unimplemented!
  return true;
  }

void SimpleModel_3DS::SetAnimation(int anim) {
  }

int SimpleModel_3DS::GetAnimation() {
  return 0;
  }
