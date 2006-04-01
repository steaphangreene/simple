// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

#include <dirent.h>

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel.h"
#include "simplemodel_q3dir.h"
#include "simplemodel_3ds.h"

#include "simplemodel_cone.h"
#include "simplemodel_cube.h"
#include "simplemodel_wedge.h"
#include "simplemodel_sphere.h"

SimpleModel *SM_LoadModel(const string &filename, const string &skinname) {
  SDL_RWops *cfg = SDL_RWFromFile((filename + "/animation.cfg").c_str(), "r");
  if(cfg) {
    SDL_RWclose(cfg);
    if(skinname.length() <= 0) return new SimpleModel_Q3Dir(filename);
    else return new SimpleModel_Q3Dir(filename, skinname);
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "3ds"))) {
    return new SimpleModel_3DS(filename, skinname);
    }      
	
  if(filename == "SimpleModel:Cone") return new SimpleModel_Cone();
  if(filename == "SimpleModel:Cube") return new SimpleModel_Cube();
  if(filename == "SimpleModel:Wedge") return new SimpleModel_Wedge();
  if(filename == "SimpleModel:Sphere") return new SimpleModel_Sphere();

  fprintf(stderr,
	"WARNING: Failed to detect model type of '%s' - using wedge.\n",
	filename.c_str());
  return new SimpleModel_Wedge();
  }

SimpleModel::SimpleModel() {
  }

SimpleModel::~SimpleModel() {
  }

bool SimpleModel::Load(const string &filenm) {
  filename = filenm;
  return false;
  }

bool SimpleModel::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const {
  return false;
  }

const vector<string> &SimpleModel::GetSkinList() {
  static vector<string> skins;
  return skins;
  }

