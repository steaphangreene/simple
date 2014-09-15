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

#ifndef	SIMPLEMODEL_PMX_H
#define	SIMPLEMODEL_PMX_H

#include "simplemodel.h"

class SimpleModel_PMX : public SimpleModel {
public:
  SimpleModel_PMX(const string &filenm,
	const string &defskin = "default");
  virtual ~SimpleModel_PMX();

  virtual bool Load(const string &filenm,
	const string &defskin = "default");

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;

  SimpleModel_PMX();
  string modelname, skinname;

  Uint8 text_encoding;
  string ReadString(SDL_RWops *model) const;

  struct PMXVertex {
    float vertex[3];
    float normal[3];
    float texture[2];
    };

  struct PMXTriangle {
    Uint32 vertex[3];
    };

  struct PMXMaterial {
    Uint8 mode;
    Uint32 texidx;
    Uint32 num_tris;
    };

  Uint32 num_tags;	// Number of tags PER FRAME
  vector<PMXVertex> vertices;
  vector<PMXTriangle> triangles;
  vector<PMXMaterial> material;
  };

#endif	//SIMPLEMODEL_PMX_H
