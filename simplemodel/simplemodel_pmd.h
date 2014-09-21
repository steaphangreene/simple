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

#ifndef	SIMPLEMODEL_PMD_H
#define	SIMPLEMODEL_PMD_H

#include "simplemodel.h"

class SimpleModel_PMD : public SimpleModel {
public:
  SimpleModel_PMD(const string &filenm,
	const string &defskin = "default");
  virtual ~SimpleModel_PMD();

  virtual bool Load(const string &filenm,
	const string &defskin = "default");

  virtual bool LoadAnimation(const string &filenm);

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;

  SimpleModel_PMD();
  string modelname, skinname;

  Uint8 text_encoding;
  string ReadString(SDL_RWops *model, size_t len) const;

  struct PMDVertex {
    float vertex[3];
    float normal[3];
    float texture[2];
    Uint16 bone[2];
    float bone_weight;
    };

  struct PMDTriangle {
    Uint32 vertex[3];
    };

  struct PMDMaterial {
    Uint32 num_tris;
    float ambient[3];
    float diffuse[4];
    float specular[3];
    float specularity;
    SimpleTexture *texture;
    };

  struct PMDBone {
    string name;
    Uint16 parent;
    Uint16 child;
    Uint8 type;
    Uint16 target;
    float pos[3];
    };

  Uint32 num_tags;	// Number of tags PER FRAME
  vector<PMDVertex> vertices;
  vector<PMDTriangle> triangles;
  vector<PMDMaterial> material;
  vector<PMDBone> bone;
  map<string,Uint16> bone_by_name;

  struct VMDBoneKeyFrame {
    float pos[3];
    Quaternion rot;
    };

  // Bone ID, Frame #, Data
  map<Uint16,map<Uint32,VMDBoneKeyFrame>> keyframe;
  };

#endif	//SIMPLEMODEL_PMD_H
