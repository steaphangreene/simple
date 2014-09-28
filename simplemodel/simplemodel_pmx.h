// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#define PMX_BONE_FLAG_TAILPOS_IS_BONE 0x0001
#define PMX_BONE_FLAG_CAN_ROTATE 0x0002
#define PMX_BONE_FLAG_CAN_TRANSLATE 0x0004
#define PMX_BONE_FLAG_IS_VISIBLE 0x0008
#define PMX_BONE_FLAG_CAN_MANIPULATE 0x0010
#define PMX_BONE_FLAG_IS_IK 0x0020
#define PMX_BONE_FLAG_IS_EXTERNAL_ROTATION 0x0100
#define PMX_BONE_FLAG_IS_EXTERNAL_TRANSLATION 0x0200
#define PMX_BONE_FLAG_HAS_FIXED_AXIS 0x0400
#define PMX_BONE_FLAG_HAS_LOCAL_COORDINATE 0x0800
#define PMX_BONE_FLAG_IS_AFTER_PHYSICS_DEFORM 0x1000
#define PMX_BONE_FLAG_IS_EXTERNAL_PARENT_DEFORM 0x2000

class SimpleModel_PMX : public SimpleModel {
public:
  SimpleModel_PMX(const string &filenm,
	const string &defskin = "default");
  virtual ~SimpleModel_PMX();

  virtual bool Load(const string &filenm,
	const string &defskin = "default");

  virtual bool LoadAnimation(const string &filenm);

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;

  SimpleModel_PMX();

  Uint8 text_encoding;
  string ReadString(SDL_RWops *model) const;
  string ReadString(SDL_RWops *model, size_t len) const;

  Uint32 ReadVarInt(SDL_RWops *model, Uint8 size) const;

  struct PMXVertex {
    float vertex[3];
    float normal[3];
    float texcoord[2];
    Uint32 bone[4];
    float bone_weight[4];
    Uint8 bone_weight_type;
    };

  struct PMXTriangle {
    Uint32 vertex[3];
    };

  struct PMXMaterial {
    Uint8 mode;
    Uint32 texidx;
    Uint32 num_tris;
    float ambient[3];
    float diffuse[4];
    float specular[3];
    float specularity;
    };

  struct PMXBone {
    string name;
    Vector3 pos;
    Uint32 parent;
    Uint16 flags;
    };

  vector<PMXVertex> vertices;
  vector<PMXTriangle> triangles;
  vector<PMXMaterial> material;
  vector<PMXBone> bone;
  map<string,Uint32> bone_by_name;

  struct VMDBoneKeyFrame {
    Vector3 pos;
    Quaternion rot;
    float bez_x[4];
    float bez_y[4];
    float bez_z[4];
    float bez_r[4];
    };

  // Bone ID, Frame #, Data
  map<Uint32,map<Uint32,VMDBoneKeyFrame>> bone_frame;
  };

#endif	//SIMPLEMODEL_PMX_H
