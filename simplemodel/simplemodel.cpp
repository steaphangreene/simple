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

#include <dirent.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
using namespace std;

#include "simplemodel.h"
#include "simplemodel_q3dir.h"
#include "simplemodel_md2.h"
#include "simplemodel_md3.h"
#include "simplemodel_mdx.h"
#include "simplemodel_3ds.h"
#include "simplemodel_obj.h"

#include "simplemodel_cone.h"
#include "simplemodel_cube.h"
#include "simplemodel_xyplane.h"
#include "simplemodel_xzplane.h"
#include "simplemodel_yzplane.h"
#include "simplemodel_wedge.h"
#include "simplemodel_sphere.h"

SimpleModel *SimpleModel::LoadModel(
	const string &filename, const string &defskin
	) {
  vector<string> skins;
  if(defskin.length() > 0) skins.push_back(defskin);
  return LoadModel(filename, skins);
  }

SimpleModel *SimpleModel::LoadModel(
	const string &filename, const vector<string> &skins
	) {
  string skin = "";
  if(skins.size() > 0) skin = skins[0];

  SDL_RWops *cfg = SDL_RWFromZZIP((filename + "/animation.cfg").c_str(), "rb");
  if(cfg) {
    SDL_RWclose(cfg);
    if(skin.length() <= 0) skin = "default";
    return new SimpleModel_Q3Dir(filename, skin);
    }
  for(Uint32 snum = 0; (!cfg) && snum < source_files.size(); ++snum) {
    cfg = SDL_RWFromZZIP((source_files[snum] + "/" + filename
	+ "/animation.cfg").c_str(), "rb");
    if(cfg) {
      SDL_RWclose(cfg);
      if(skin.length() <= 0) skin = "default";
      return new SimpleModel_Q3Dir(source_files[snum], filename, skin);
      }
    }

  SDL_RWops *md2 = SDL_RWFromZZIP((filename + "/tris.md2").c_str(), "rb");
  if(md2) {
    SDL_RWclose(md2);
    return new SimpleModel_MD2(filename, "tris.md2", skin);
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "obj"))) {
    return new SimpleModel_OBJ(filename);
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "3ds"))) {
    return new SimpleModel_3DS(filename);
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "md3"))) {
    SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
    if(skin.length() <= 0) skin = "default";
    if(model) {
      SDL_RWclose(model);
      if(filename[0] == '/') {
	return new SimpleModel_MD3("/", filename, skin);
	}
      return new SimpleModel_MD3(".", filename, skin);
      }
    for(Uint32 snum = 0; snum < source_files.size(); ++snum) {
      model = SDL_RWFromZZIP((source_files[snum] + "/" + filename).c_str(), "rb");
      if(model) {
	SDL_RWclose(model);
	return new SimpleModel_MD3(source_files[snum], filename, skin);
	}
      }
    fprintf(stderr, "WARNING: Failed to load md3 model '%s' - using wedge.\n",
	filename.c_str());
    return new SimpleModel_Wedge();
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "md2"))) {
    SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
    if(model) {
      SDL_RWclose(model);
      if(filename[0] == '/') {
	return new SimpleModel_MD2("/", filename, skin);
	}
      return new SimpleModel_MD2(".", filename, skin);
      }
    for(Uint32 snum = 0; snum < source_files.size(); ++snum) {
      model = SDL_RWFromZZIP((source_files[snum] + "/" + filename).c_str(), "rb");
      if(model) {
	SDL_RWclose(model);
	return new SimpleModel_MD2(source_files[snum], filename, skin);
	}
      }
    fprintf(stderr, "WARNING: Failed to load md2 model '%s' - using wedge.\n",
	filename.c_str());
    return new SimpleModel_Wedge();
    }

  if(filename.length() >= 3
	&& (!strcasecmp(filename.c_str() + filename.length() - 3, "mdx"))) {
    SDL_RWops *model = SDL_RWFromZZIP(filename.c_str(), "rb");
    if(model) {
      SDL_RWclose(model);
      if(filename[0] == '/') {
	return new SimpleModel_MDX("/", filename, skins);
	}
      return new SimpleModel_MDX(".", filename, skins);
      }
    for(Uint32 snum = 0; snum < source_files.size(); ++snum) {
      model = SDL_RWFromZZIP((source_files[snum] + "/" + filename).c_str(), "rb");
      if(model) {
	SDL_RWclose(model);
	return new SimpleModel_MDX(source_files[snum], filename, skins);
	}
      }
    fprintf(stderr, "WARNING: Failed to load mdx model '%s' - using wedge.\n",
	filename.c_str());
    return new SimpleModel_Wedge();
    }

  if(filename == "SimpleModel:Cone") return new SimpleModel_Cone();
  if(filename == "SimpleModel:Cube") return new SimpleModel_Cube();
  if(filename == "SimpleModel:Wedge") return new SimpleModel_Wedge();
  if(filename == "SimpleModel:Sphere") return new SimpleModel_Sphere();
  if(filename == "SimpleModel:XYPlane") return new SimpleModel_XYPlane();
  if(filename == "SimpleModel:XZPlane") return new SimpleModel_XZPlane();
  if(filename == "SimpleModel:YZPlane") return new SimpleModel_YZPlane();

  fprintf(stderr,
	"WARNING: Failed to detect model type of '%s' - using wedge.\n",
	filename.c_str());
  return new SimpleModel_Wedge();
  }

SimpleModel::SimpleModel() {
  if(!gl_ext_detected) {
    SetupGLEXT();
    }
  }

SimpleModel::~SimpleModel() {
  }

bool SimpleModel::Load(const string &filenm, const vector<string> &skin) {
  filename = filenm;
  return false;
  }

bool SimpleModel::MoveToTag(const Uint32 tag, Uint32 cur_time,
        const vector<int> &anim, const vector<Uint32> &start_time,
	Uint32 anim_offset) const {
  return false;
  }

Uint32 SimpleModel::TagNameToIndex(const string &tagname) const {
  if(tags.count(tagname) < 1) {
    map<Uint32, SimpleModel*>::const_iterator itr = submodels.begin();
    for(; itr != submodels.end(); ++itr) {
      Uint32 ret = itr->second->TagNameToIndex(tagname);
      if(ret != 0xFFFFFFFF) return ((ret << 8) | itr->first);
      }
    return (Uint32)0xFFFFFFFF;
    }
  return tags.find(tagname)->second;
  }

bool SimpleModel::MoveToTag(const string &tagname, Uint32 cur_time,
	const vector<int> &anim, const vector<Uint32> &start_time,
	Uint32 anim_offset) const {
  return MoveToTag(TagNameToIndex(tagname), cur_time, anim, start_time);
  }


bool SimpleModel::Render(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  if(!RenderSelf(cur_time, anim, start_time, anim_offset)) return false;
  map<Uint32, SimpleModel*>::const_iterator itr = submodels.begin();
  for(; itr != submodels.end(); ++itr) {
    bool ret = false;
    glPushMatrix();
    if(MoveToTag(itr->first, cur_time, anim, start_time, anim_offset)) {
      if(itr->second->Render(cur_time, anim, start_time,
		anim_offset + tag_anim_offsets.find(itr->first)->second)) {
	ret = true;
	}
      }
    glPopMatrix();
    }
  return true;
  }

bool SimpleModel::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  return false;	//ERROR - Can't Render Abstract Class!
  }

const vector<string> &SimpleModel::GetSkinList() {
  static vector<string> skins;
  return skins;
  }

void SimpleModel::Normalize(Quaternion &res, const Quaternion &quat) {
  float scale = sqrtf(quat.data[0]*quat.data[0]
	+ quat.data[1]*quat.data[1] + quat.data[2]*quat.data[2]
	+ quat.data[3]*quat.data[3]);
  res.data[0] = quat.data[0] / scale;
  res.data[1] = quat.data[1] / scale;
  res.data[2] = quat.data[2] / scale;
  res.data[3] = quat.data[3] / scale;
  }

void SimpleModel::SLERP(Matrix4x4 &res,
	const Matrix4x4 &m1, const Matrix4x4 &m2, const float t) {
  Quaternion q1, q2, qres;
  Matrix4x4ToQuaternion(q1, m1);
  Matrix4x4ToQuaternion(q2, m2);
  SLERP(qres, q1, q2, t);
  QuaternionToMatrix4x4(res, qres);
  res.data[12] = (1.0f - t) * m1.data[12] + t * m2.data[12];
  res.data[13] = (1.0f - t) * m1.data[13] + t * m2.data[13];
  res.data[14] = (1.0f - t) * m1.data[14] + t * m2.data[14];
  }

void SimpleModel::Multiply(Matrix4x4 &res,
	const Matrix4x4 &m1, const Matrix4x4 &m2) {
  res.data[0] = m1.data[0]*m2.data[0] + m1.data[4]*m2.data[1]
	+ m1.data[8]*m2.data[2] + m1.data[12]*m2.data[3];
  res.data[1] = m1.data[1]*m2.data[0] + m1.data[5]*m2.data[1]
	+ m1.data[9]*m2.data[2] + m1.data[13]*m2.data[3];
  res.data[2] = m1.data[2]*m2.data[0] + m1.data[6]*m2.data[1]
	+ m1.data[10]*m2.data[2] + m1.data[14]*m2.data[3];
  res.data[3] = m1.data[3]*m2.data[0] + m1.data[7]*m2.data[1]
	+ m1.data[11]*m2.data[2] + m1.data[15]*m2.data[3];
  res.data[4] = m1.data[0]*m2.data[4] + m1.data[4]*m2.data[5]
	+ m1.data[8]*m2.data[6] + m1.data[12]*m2.data[7];
  res.data[5] = m1.data[1]*m2.data[4] + m1.data[5]*m2.data[5]
	+ m1.data[9]*m2.data[6] + m1.data[13]*m2.data[7];
  res.data[6] = m1.data[2]*m2.data[4] + m1.data[6]*m2.data[5]
	+ m1.data[10]*m2.data[6] + m1.data[14]*m2.data[7];
  res.data[7] = m1.data[3]*m2.data[4] + m1.data[7]*m2.data[5]
	+ m1.data[11]*m2.data[6] + m1.data[15]*m2.data[7];
  res.data[8] = m1.data[0]*m2.data[8] + m1.data[4]*m2.data[9]
	+ m1.data[8]*m2.data[10] + m1.data[12]*m2.data[11];
  res.data[9] = m1.data[1]*m2.data[8] + m1.data[5]*m2.data[9]
	+ m1.data[9]*m2.data[10] + m1.data[13]*m2.data[11];
  res.data[10] = m1.data[2]*m2.data[8] + m1.data[6]*m2.data[9]
	+ m1.data[10]*m2.data[10] + m1.data[14]*m2.data[11];
  res.data[11] = m1.data[3]*m2.data[8] + m1.data[7]*m2.data[9]
	+ m1.data[11]*m2.data[10] + m1.data[15]*m2.data[11];
  res.data[12] = m1.data[0]*m2.data[12] + m1.data[4]*m2.data[13]
	+ m1.data[8]*m2.data[14] + m1.data[12]*m2.data[15];
  res.data[13] = m1.data[1]*m2.data[12] + m1.data[5]*m2.data[13]
	+ m1.data[9]*m2.data[14] + m1.data[13]*m2.data[15];
  res.data[14] = m1.data[2]*m2.data[12] + m1.data[6]*m2.data[13]
	+ m1.data[10]*m2.data[14] + m1.data[14]*m2.data[15];
  res.data[15] = m1.data[3]*m2.data[12] + m1.data[7]*m2.data[13]
	+ m1.data[11]*m2.data[14] + m1.data[15]*m2.data[15];
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3) {
  //This may not look better, but GCC will be able to optimize this much more!
  res.data[0] = m1.data[0]*(m2.data[0]*m3.data[0] + m2.data[4]*m3.data[1] + m2.data[8]*m3.data[2] + m2.data[12]*m3.data[3]) + m1.data[4]*(m2.data[1]*m3.data[0] + m2.data[5]*m3.data[1] + m2.data[9]*m3.data[2] + m2.data[13]*m3.data[3]) + m1.data[8]*(m2.data[2]*m3.data[0] + m2.data[6]*m3.data[1] + m2.data[10]*m3.data[2] + m2.data[14]*m3.data[3]) + m1.data[12]*(m2.data[3]*m3.data[0] + m2.data[7]*m3.data[1] + m2.data[11]*m3.data[2] + m2.data[15]*m3.data[3]);
  res.data[1] = m1.data[1]*(m2.data[0]*m3.data[0] + m2.data[4]*m3.data[1] + m2.data[8]*m3.data[2] + m2.data[12]*m3.data[3]) + m1.data[5]*(m2.data[1]*m3.data[0] + m2.data[5]*m3.data[1] + m2.data[9]*m3.data[2] + m2.data[13]*m3.data[3]) + m1.data[9]*(m2.data[2]*m3.data[0] + m2.data[6]*m3.data[1] + m2.data[10]*m3.data[2] + m2.data[14]*m3.data[3]) + m1.data[13]*(m2.data[3]*m3.data[0] + m2.data[7]*m3.data[1] + m2.data[11]*m3.data[2] + m2.data[15]*m3.data[3]);
  res.data[2] = m1.data[2]*(m2.data[0]*m3.data[0] + m2.data[4]*m3.data[1] + m2.data[8]*m3.data[2] + m2.data[12]*m3.data[3]) + m1.data[6]*(m2.data[1]*m3.data[0] + m2.data[5]*m3.data[1] + m2.data[9]*m3.data[2] + m2.data[13]*m3.data[3]) + m1.data[10]*(m2.data[2]*m3.data[0] + m2.data[6]*m3.data[1] + m2.data[10]*m3.data[2] + m2.data[14]*m3.data[3]) + m1.data[14]*(m2.data[3]*m3.data[0] + m2.data[7]*m3.data[1] + m2.data[11]*m3.data[2] + m2.data[15]*m3.data[3]);
  res.data[3] = m1.data[3]*(m2.data[0]*m3.data[0] + m2.data[4]*m3.data[1] + m2.data[8]*m3.data[2] + m2.data[12]*m3.data[3]) + m1.data[7]*(m2.data[1]*m3.data[0] + m2.data[5]*m3.data[1] + m2.data[9]*m3.data[2] + m2.data[13]*m3.data[3]) + m1.data[11]*(m2.data[2]*m3.data[0] + m2.data[6]*m3.data[1] + m2.data[10]*m3.data[2] + m2.data[14]*m3.data[3]) + m1.data[15]*(m2.data[3]*m3.data[0] + m2.data[7]*m3.data[1] + m2.data[11]*m3.data[2] + m2.data[15]*m3.data[3]);
  res.data[4] = m1.data[0]*(m2.data[0]*m3.data[4] + m2.data[4]*m3.data[5] + m2.data[8]*m3.data[6] + m2.data[12]*m3.data[7]) + m1.data[4]*(m2.data[1]*m3.data[4] + m2.data[5]*m3.data[5] + m2.data[9]*m3.data[6] + m2.data[13]*m3.data[7]) + m1.data[8]*(m2.data[2]*m3.data[4] + m2.data[6]*m3.data[5] + m2.data[10]*m3.data[6] + m2.data[14]*m3.data[7]) + m1.data[12]*(m2.data[3]*m3.data[4] + m2.data[7]*m3.data[5] + m2.data[11]*m3.data[6] + m2.data[15]*m3.data[7]);
  res.data[5] = m1.data[1]*(m2.data[0]*m3.data[4] + m2.data[4]*m3.data[5] + m2.data[8]*m3.data[6] + m2.data[12]*m3.data[7]) + m1.data[5]*(m2.data[1]*m3.data[4] + m2.data[5]*m3.data[5] + m2.data[9]*m3.data[6] + m2.data[13]*m3.data[7]) + m1.data[9]*(m2.data[2]*m3.data[4] + m2.data[6]*m3.data[5] + m2.data[10]*m3.data[6] + m2.data[14]*m3.data[7]) + m1.data[13]*(m2.data[3]*m3.data[4] + m2.data[7]*m3.data[5] + m2.data[11]*m3.data[6] + m2.data[15]*m3.data[7]);
  res.data[6] = m1.data[2]*(m2.data[0]*m3.data[4] + m2.data[4]*m3.data[5] + m2.data[8]*m3.data[6] + m2.data[12]*m3.data[7]) + m1.data[6]*(m2.data[1]*m3.data[4] + m2.data[5]*m3.data[5] + m2.data[9]*m3.data[6] + m2.data[13]*m3.data[7]) + m1.data[10]*(m2.data[2]*m3.data[4] + m2.data[6]*m3.data[5] + m2.data[10]*m3.data[6] + m2.data[14]*m3.data[7]) + m1.data[14]*(m2.data[3]*m3.data[4] + m2.data[7]*m3.data[5] + m2.data[11]*m3.data[6] + m2.data[15]*m3.data[7]);
  res.data[7] = m1.data[3]*(m2.data[0]*m3.data[4] + m2.data[4]*m3.data[5] + m2.data[8]*m3.data[6] + m2.data[12]*m3.data[7]) + m1.data[7]*(m2.data[1]*m3.data[4] + m2.data[5]*m3.data[5] + m2.data[9]*m3.data[6] + m2.data[13]*m3.data[7]) + m1.data[11]*(m2.data[2]*m3.data[4] + m2.data[6]*m3.data[5] + m2.data[10]*m3.data[6] + m2.data[14]*m3.data[7]) + m1.data[15]*(m2.data[3]*m3.data[4] + m2.data[7]*m3.data[5] + m2.data[11]*m3.data[6] + m2.data[15]*m3.data[7]);
  res.data[8] = m1.data[0]*(m2.data[0]*m3.data[8] + m2.data[4]*m3.data[9] + m2.data[8]*m3.data[10] + m2.data[12]*m3.data[11]) + m1.data[4]*(m2.data[1]*m3.data[8] + m2.data[5]*m3.data[9] + m2.data[9]*m3.data[10] + m2.data[13]*m3.data[11]) + m1.data[8]*(m2.data[2]*m3.data[8] + m2.data[6]*m3.data[9] + m2.data[10]*m3.data[10] + m2.data[14]*m3.data[11]) + m1.data[12]*(m2.data[3]*m3.data[8] + m2.data[7]*m3.data[9] + m2.data[11]*m3.data[10] + m2.data[15]*m3.data[11]);
  res.data[9] = m1.data[1]*(m2.data[0]*m3.data[8] + m2.data[4]*m3.data[9] + m2.data[8]*m3.data[10] + m2.data[12]*m3.data[11]) + m1.data[5]*(m2.data[1]*m3.data[8] + m2.data[5]*m3.data[9] + m2.data[9]*m3.data[10] + m2.data[13]*m3.data[11]) + m1.data[9]*(m2.data[2]*m3.data[8] + m2.data[6]*m3.data[9] + m2.data[10]*m3.data[10] + m2.data[14]*m3.data[11]) + m1.data[13]*(m2.data[3]*m3.data[8] + m2.data[7]*m3.data[9] + m2.data[11]*m3.data[10] + m2.data[15]*m3.data[11]);
  res.data[10] = m1.data[2]*(m2.data[0]*m3.data[8] + m2.data[4]*m3.data[9] + m2.data[8]*m3.data[10] + m2.data[12]*m3.data[11]) + m1.data[6]*(m2.data[1]*m3.data[8] + m2.data[5]*m3.data[9] + m2.data[9]*m3.data[10] + m2.data[13]*m3.data[11]) + m1.data[10]*(m2.data[2]*m3.data[8] + m2.data[6]*m3.data[9] + m2.data[10]*m3.data[10] + m2.data[14]*m3.data[11]) + m1.data[14]*(m2.data[3]*m3.data[8] + m2.data[7]*m3.data[9] + m2.data[11]*m3.data[10] + m2.data[15]*m3.data[11]);
  res.data[11] = m1.data[3]*(m2.data[0]*m3.data[8] + m2.data[4]*m3.data[9] + m2.data[8]*m3.data[10] + m2.data[12]*m3.data[11]) + m1.data[7]*(m2.data[1]*m3.data[8] + m2.data[5]*m3.data[9] + m2.data[9]*m3.data[10] + m2.data[13]*m3.data[11]) + m1.data[11]*(m2.data[2]*m3.data[8] + m2.data[6]*m3.data[9] + m2.data[10]*m3.data[10] + m2.data[14]*m3.data[11]) + m1.data[15]*(m2.data[3]*m3.data[8] + m2.data[7]*m3.data[9] + m2.data[11]*m3.data[10] + m2.data[15]*m3.data[11]);
  res.data[12] = m1.data[0]*(m2.data[0]*m3.data[12] + m2.data[4]*m3.data[13] + m2.data[8]*m3.data[14] + m2.data[12]*m3.data[15]) + m1.data[4]*(m2.data[1]*m3.data[12] + m2.data[5]*m3.data[13] + m2.data[9]*m3.data[14] + m2.data[13]*m3.data[15]) + m1.data[8]*(m2.data[2]*m3.data[12] + m2.data[6]*m3.data[13] + m2.data[10]*m3.data[14] + m2.data[14]*m3.data[15]) + m1.data[12]*(m2.data[3]*m3.data[12] + m2.data[7]*m3.data[13] + m2.data[11]*m3.data[14] + m2.data[15]*m3.data[15]);
  res.data[13] = m1.data[1]*(m2.data[0]*m3.data[12] + m2.data[4]*m3.data[13] + m2.data[8]*m3.data[14] + m2.data[12]*m3.data[15]) + m1.data[5]*(m2.data[1]*m3.data[12] + m2.data[5]*m3.data[13] + m2.data[9]*m3.data[14] + m2.data[13]*m3.data[15]) + m1.data[9]*(m2.data[2]*m3.data[12] + m2.data[6]*m3.data[13] + m2.data[10]*m3.data[14] + m2.data[14]*m3.data[15]) + m1.data[13]*(m2.data[3]*m3.data[12] + m2.data[7]*m3.data[13] + m2.data[11]*m3.data[14] + m2.data[15]*m3.data[15]);
  res.data[14] = m1.data[2]*(m2.data[0]*m3.data[12] + m2.data[4]*m3.data[13] + m2.data[8]*m3.data[14] + m2.data[12]*m3.data[15]) + m1.data[6]*(m2.data[1]*m3.data[12] + m2.data[5]*m3.data[13] + m2.data[9]*m3.data[14] + m2.data[13]*m3.data[15]) + m1.data[10]*(m2.data[2]*m3.data[12] + m2.data[6]*m3.data[13] + m2.data[10]*m3.data[14] + m2.data[14]*m3.data[15]) + m1.data[14]*(m2.data[3]*m3.data[12] + m2.data[7]*m3.data[13] + m2.data[11]*m3.data[14] + m2.data[15]*m3.data[15]);
  res.data[15] = m1.data[3]*(m2.data[0]*m3.data[12] + m2.data[4]*m3.data[13] + m2.data[8]*m3.data[14] + m2.data[12]*m3.data[15]) + m1.data[7]*(m2.data[1]*m3.data[12] + m2.data[5]*m3.data[13] + m2.data[9]*m3.data[14] + m2.data[13]*m3.data[15]) + m1.data[11]*(m2.data[2]*m3.data[12] + m2.data[6]*m3.data[13] + m2.data[10]*m3.data[14] + m2.data[14]*m3.data[15]) + m1.data[15]*(m2.data[3]*m3.data[12] + m2.data[7]*m3.data[13] + m2.data[11]*m3.data[14] + m2.data[15]*m3.data[15]);
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3, const Matrix4x4 &m4) {
  Matrix4x4 temp;
  Multiply(temp, m2, m3, m4);
  Multiply(res, m1, temp);
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3, const Matrix4x4 &m4,
	const Matrix4x4 &m5) {
  Matrix4x4 temp;
  Multiply(temp, m3, m4, m5);
  Multiply(res, m1, m2, temp);
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3, const Matrix4x4 &m4,
	const Matrix4x4 &m5, const Matrix4x4 &m6) {
  Matrix4x4 temp;
  Multiply(temp, m3, m4, m5, m6);
  Multiply(res, m1, m2, temp);
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3, const Matrix4x4 &m4,
	const Matrix4x4 &m5, const Matrix4x4 &m6, const Matrix4x4 &m7) {
  Matrix4x4 temp, temp2;
  Multiply(temp, m5, m6, m7);
  Multiply(temp2, m3, m4, temp);
  Multiply(res, m1, m2, temp);
  }

void SimpleModel::Multiply(Matrix4x4 &res, const Matrix4x4 &m1,
	const Matrix4x4 &m2, const Matrix4x4 &m3, const Matrix4x4 &m4,
	const Matrix4x4 &m5, const Matrix4x4 &m6, const Matrix4x4 &m7,
	const Matrix4x4 &m8) {
  Matrix4x4 temp, temp2;
  Multiply(temp, m5, m6, m7, m8);
  Multiply(temp2, m3, m4, temp);
  Multiply(res, m1, m2, temp);
  }

void SimpleModel::QuaternionToMatrix4x4(Matrix4x4 &mat, const Quaternion &quat) {
  mat.data[0] = 1.0f - 2.0f *
	(quat.data[1] * quat.data[1] + quat.data[2] * quat.data[2]);
  mat.data[1] = 2.0f *
	(quat.data[0] * quat.data[1] - quat.data[3] * quat.data[2]);
  mat.data[2] = 2.0f *
	(quat.data[0] * quat.data[2] + quat.data[3] * quat.data[1]);
  mat.data[3] = 0.0f;

  mat.data[4] = 2.0f *
	(quat.data[0] * quat.data[1] + quat.data[3] * quat.data[2]);
  mat.data[5] = 1.0f - 2.0f *
	(quat.data[0] * quat.data[0] + quat.data[2] * quat.data[2]);
  mat.data[6] = 2.0f *
	(quat.data[1] * quat.data[2] - quat.data[3] * quat.data[0]);
  mat.data[7] = 0.0f;

  mat.data[8] = 2.0f *
	(quat.data[0] * quat.data[2] - quat.data[3] * quat.data[1]);
  mat.data[9] = 2.0f *
	(quat.data[1] * quat.data[2] + quat.data[3] * quat.data[0]);
  mat.data[10] = 1.0f - 2.0f *
	(quat.data[0] * quat.data[0] + quat.data[1] * quat.data[1]);
  mat.data[11] = 0.0f;

  mat.data[12] = 0;
  mat.data[13] = 0;
  mat.data[14] = 0;
  mat.data[15] = 1.0f;
  }

void SimpleModel::Matrix4x4ToQuaternion(Quaternion &quat, const Matrix4x4 &mat) {
  float trace, scale;

  trace = 1 + mat.data[0] + mat.data[5] + mat.data[10];

  if(trace > 0.00000001f) {
    scale = sqrt(trace) * 2.0f;
    quat.data[0] = (mat.data[9] - mat.data[6]) / scale;
    quat.data[1] = (mat.data[2] - mat.data[8]) / scale;
    quat.data[2] = (mat.data[4] - mat.data[1]) / scale;
    quat.data[3] = 0.25 * scale;
    }
  else {
    if(mat.data[0] > mat.data[5] && mat.data[0] > mat.data[10]) {
      scale  = sqrt(1.0f + mat.data[0] - mat.data[5] - mat.data[10]) * 2.0f;
      quat.data[0] = 0.25f * scale;
      quat.data[1] = (mat.data[1] + mat.data[4]) / scale;
      quat.data[2] = (mat.data[2] + mat.data[8]) / scale;
      quat.data[3] = (mat.data[9] - mat.data[6]) / scale;
      }
    else if(mat.data[5] > mat.data[10]) {
      scale  = sqrt(1.0f + mat.data[5] - mat.data[0] - mat.data[10]) * 2.0f;
      quat.data[0] = (mat.data[1] + mat.data[4]) / scale;
      quat.data[1] = 0.25f * scale;
      quat.data[2] = (mat.data[6] + mat.data[9]) / scale;
      quat.data[3] = (mat.data[2] - mat.data[8]) / scale;
      }
    else {
      scale  = sqrt(1.0f + mat.data[10] - mat.data[0] - mat.data[5]) * 2.0f;
      quat.data[0] = (mat.data[2] + mat.data[8]) / scale;
      quat.data[1] = (mat.data[6] + mat.data[9]) / scale;
      quat.data[2] = 0.25f * scale;
      quat.data[3] = (mat.data[4] - mat.data[1]) / scale;
      }
    }
  }


void SimpleModel::SLERP(Quaternion &res,
	const Quaternion &q1, const Quaternion &q2, const float t) {
  float cos_theta, ab_cos_theta, s1, s2;

  cos_theta = q1.data[1] * q2.data[1] + q1.data[2] * q2.data[2]
	+ q1.data[3] * q2.data[3] + q1.data[0] * q2.data[0];
  ab_cos_theta = fabsf(cos_theta);

  // Calculate the coefficients s1 and s2
  if(ab_cos_theta < 0.9f) {	// Not close, do full-blown SLERP
    float theta = acos(ab_cos_theta);
    float sin_theta = sin(theta);
    s1 = sin((1.0 - t) * theta) / sin_theta;
    s2 = sin(t * theta) / sin_theta;
    }
  else {			// Pretty close, go linear
    s1 = 1.0 - t;
    s2 = t;
    }
  if(cos_theta < 0.0) {
    res.data[0] = s1 * q1.data[0] - s2 * q2.data[0];	// W
    res.data[1] = s1 * q1.data[1] - s2 * q2.data[1];	// X
    res.data[2] = s1 * q1.data[2] - s2 * q2.data[2];	// Y
    res.data[3] = s1 * q1.data[3] - s2 * q2.data[3];	// Z
    }
  else {
    res.data[0] = s1 * q1.data[0] + s2 * q2.data[0];	// W
    res.data[1] = s1 * q1.data[1] + s2 * q2.data[1];	// X
    res.data[2] = s1 * q1.data[2] + s2 * q2.data[2];	// Y
    res.data[3] = s1 * q1.data[3] + s2 * q2.data[3];	// Z
    }
  }

void SimpleModel::AddSourceFile(const string &in) {
  source_files.push_back(in);
  }

void SimpleModel::ClearSourceFiles() {
  source_files.clear();
  }

const vector<string> SimpleModel::GetSourceFiles() {
  return source_files;
  }

vector<string> SimpleModel::source_files;

SimpleModel *SM_LoadModel(const string &filename, const string &defskin) {
  vector<string> skins;
  if(defskin.length() > 0) skins.push_back(defskin);
  return SimpleModel::LoadModel(filename, skins);
  }

SimpleModel *SM_LoadModel(const string &filename, const vector<string> &skins) {
  return SimpleModel::LoadModel(filename, skins);
  }

SimpleModel::Matrix4x4 SimpleModel::identity4x4 = {{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
	}};

void SimpleModel::AttachSubmodel(Uint32 tag, SimpleModel *submodel) {
  if((tag & 0xFFFFFF00) != 0) {
    if(submodels.count(tag&0xFF) > 0) {
      submodels[tag&0xFF]->AttachSubmodel(tag>>8, submodel);
      }
    }
  else {
    submodels.insert(pair<Uint32, SimpleModel *>(tag, submodel));
    if(tag_anim_offsets.count(tag) < 1) tag_anim_offsets[tag] = 0;
    }
  }

void SimpleModel::DetachSubmodel(const SimpleModel *submodel) {
  map<Uint32, SimpleModel *>::iterator itr = submodels.begin();
  for(; itr != submodels.end(); ++itr) {
    if(itr->second == submodel) {
      submodels.erase(itr);
      break;
      }
    }
  }

void SimpleModel::AttachSubmodel(const string &tag, SimpleModel *submodel) {
  AttachSubmodel(TagNameToIndex(tag), submodel);
  }

void SimpleModel::DetachSubmodel(Uint32 tag) {
  submodels.erase(tag);
  }

void SimpleModel::DetachSubmodel(const string &tag) {
  submodels.erase(TagNameToIndex(tag));
  }

void SimpleModel::SetAnimOffset(Uint32 tag, Uint32 offset) {
  tag_anim_offsets[tag] = offset;
  }

void SimpleModel::SetAnimOffset(const string &tag, Uint32 offset) {
  SetAnimOffset(TagNameToIndex(tag), offset);
  }

int SimpleModel::LookUpAnimation(const string &anim) {
  if(!animations.count(anim)) return -1;
  return animations.find(anim)->second;
  }

bool SimpleModel::gl_ext_detected = false;
SimpleModel::glGenVBO SimpleModel::glGenBuffersARB = NULL;
SimpleModel::glBndVBO SimpleModel::glBindBufferARB = NULL;
SimpleModel::glBufVBO SimpleModel::glBufferDataARB = NULL;
SimpleModel::glDelVBO SimpleModel::glDeleteBuffersARB = NULL;
SimpleModel::glIsAVBO SimpleModel::glIsBufferARB = NULL;
SimpleModel::glActTEX SimpleModel::glActiveTextureARB = NULL;
SimpleModel::glMTXCRD2f SimpleModel::glMultiTexCoord2fARB = NULL;

void SimpleModel::SetupGLEXT() {
  SimpleModel::glGenBuffersARB =
	(glGenVBO) SDL_GL_GetProcAddress("glGenBuffersARB");
  SimpleModel::glBindBufferARB =
	(glBndVBO) SDL_GL_GetProcAddress("glBindBufferARB");
  SimpleModel::glBufferDataARB =
	(glBufVBO) SDL_GL_GetProcAddress("glBufferDataARB");
  SimpleModel::glDeleteBuffersARB =
	(glDelVBO) SDL_GL_GetProcAddress("glDeleteBuffersARB");
  SimpleModel::glIsBufferARB =
	(glIsAVBO) SDL_GL_GetProcAddress("glIsBufferARB");
  if(SimpleModel::glGenBuffersARB == NULL
	|| SimpleModel::glBindBufferARB == NULL
	|| SimpleModel::glBufferDataARB == NULL
	|| SimpleModel::glDeleteBuffersARB == NULL
	|| SimpleModel::glIsBufferARB == NULL) {
    SimpleModel::glGenBuffersARB = NULL;
    SimpleModel::glBindBufferARB = NULL;
    SimpleModel::glBufferDataARB = NULL;
    SimpleModel::glDeleteBuffersARB = NULL;
    SimpleModel::glIsBufferARB = NULL;
    }
  SimpleModel::glActiveTextureARB =
	(glActTEX) SDL_GL_GetProcAddress("glActiveTextureARB");
  SimpleModel::glMultiTexCoord2fARB =
	(glMTXCRD2f) SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
  gl_ext_detected = true;
  }
