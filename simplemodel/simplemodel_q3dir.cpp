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

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel_q3dir.h"

SimpleModel_Q3Dir::SimpleModel_Q3Dir(const string &filenm) {
  head = NULL;
  legs = NULL;
  torso = NULL;
  weapon = NULL;
  Load(filenm);
  }

SimpleModel_Q3Dir::SimpleModel_Q3Dir() {
  head = NULL;
  legs = NULL;
  torso = NULL;
  weapon = NULL;
  }

SimpleModel_Q3Dir::~SimpleModel_Q3Dir() {
  if(head) delete head;
  if(legs) delete legs;
  if(torso) delete torso;
  if(weapon) weapon = NULL;	// Not mine!
  }

bool SimpleModel_Q3Dir::Load(const string &filenm) {
  filename = filenm;
  head = new SimpleModel_Md3(
	filename, filename + "/head.md3", filename + "/head_default.skin");
  legs = new SimpleModel_Md3(
	filename, filename + "/lower.md3", filename + "/lower_default.skin");
  torso = new SimpleModel_Md3(
	filename, filename + "/upper.md3", filename + "/upper_default.skin");

  //FIXME: Set Initial Animation

  return false;
  }

bool SimpleModel_Q3Dir::Render(Uint32 cur_time) {
  //FIXME: Attach submodels to tags
  //FIXME: Interpolate Models Using Animations
  //FIXME: Interpolate Tags
  legs->Render(cur_time);
  torso->Render(cur_time);
  head->Render(cur_time);
  return false;
  }
