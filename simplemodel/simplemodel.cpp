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

#include <cmath>
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

SimpleModel *SimpleModel::LoadModel(const string &filename, const string &skinname) {
  SDL_RWops *cfg = SDL_RWFromZZIP((filename + "/animation.cfg").c_str(), "rb");
  if(cfg) {
    string skin = skinname;
    SDL_RWclose(cfg);
    if(skin.length() <= 0) skin = "default";
    return new SimpleModel_Q3Dir(filename, skin);
    }
  for(Uint32 snum = 0; (!cfg) && snum < source_files.size(); ++snum) {
    cfg = SDL_RWFromZZIP((source_files[snum] + "/" + filename
	+ "/animation.cfg").c_str(), "rb");
    if(cfg) {
      string skin = skinname;
      SDL_RWclose(cfg);
      if(skin.length() <= 0) skin = "default";
      return new SimpleModel_Q3Dir(source_files[snum], filename, skin);
      }
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

void SimpleModel::SLERP(Matrix4x4 &res,
	const Matrix4x4 &m1, const Matrix4x4 &m2, const float t) {
  Quaternion q1 = {{0.0}}, q2 = {{0.0}}, qres = {{0.0}};
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
  res.data[4] = m1.data[0]*m2.data[4] + m1.data[4]*m2.data[5]
	+ m1.data[8]*m2.data[6] + m1.data[12]*m2.data[7];
  res.data[8] = m1.data[0]*m2.data[8] + m1.data[4]*m2.data[9]
	+ m1.data[8]*m2.data[10] + m1.data[12]*m2.data[11];
  res.data[12] = m1.data[0]*m2.data[12] + m1.data[4]*m2.data[13]
	+ m1.data[8]*m2.data[14] + m1.data[12]*m2.data[15];
  res.data[1] = m1.data[1]*m2.data[0] + m1.data[5]*m2.data[1]
	+ m1.data[9]*m2.data[2] + m1.data[13]*m2.data[3];
  res.data[5] = m1.data[1]*m2.data[4] + m1.data[5]*m2.data[5]
	+ m1.data[9]*m2.data[6] + m1.data[13]*m2.data[7];
  res.data[9] = m1.data[1]*m2.data[8] + m1.data[5]*m2.data[9]
	+ m1.data[9]*m2.data[10] + m1.data[13]*m2.data[11];
  res.data[13] = m1.data[1]*m2.data[12] + m1.data[5]*m2.data[13]
	+ m1.data[9]*m2.data[14] + m1.data[13]*m2.data[15];
  res.data[2] = m1.data[2]*m2.data[0] + m1.data[6]*m2.data[1]
	+ m1.data[10]*m2.data[2] + m1.data[14]*m2.data[3];
  res.data[6] = m1.data[2]*m2.data[4] + m1.data[6]*m2.data[5]
	+ m1.data[10]*m2.data[6] + m1.data[14]*m2.data[7];
  res.data[10] = m1.data[2]*m2.data[8] + m1.data[6]*m2.data[9]
	+ m1.data[10]*m2.data[10] + m1.data[14]*m2.data[11];
  res.data[14] = m1.data[2]*m2.data[12] + m1.data[6]*m2.data[13]
	+ m1.data[10]*m2.data[14] + m1.data[14]*m2.data[15];
  res.data[3] = m1.data[3]*m2.data[0] + m1.data[7]*m2.data[1]
	+ m1.data[11]*m2.data[2] + m1.data[15]*m2.data[3];
  res.data[7] = m1.data[3]*m2.data[4] + m1.data[7]*m2.data[5]
	+ m1.data[11]*m2.data[6] + m1.data[15]*m2.data[7];
  res.data[11] = m1.data[3]*m2.data[8] + m1.data[7]*m2.data[9]
	+ m1.data[11]*m2.data[10] + m1.data[15]*m2.data[11];
  res.data[15] = m1.data[3]*m2.data[12] + m1.data[7]*m2.data[13]
	+ m1.data[11]*m2.data[14] + m1.data[15]*m2.data[15];
  }

void SimpleModel::QuaternionToMatrix4x4(Matrix4x4 &mat, const Quaternion &quat) {
  mat.data[0] = 1.0f - 2.0f *
	(quat.data[2] * quat.data[2] + quat.data[3] * quat.data[3]);
  mat.data[1] = 2.0f *
	(quat.data[1] * quat.data[2] - quat.data[0] * quat.data[3]);
  mat.data[2] = 2.0f *
	(quat.data[1] * quat.data[3] + quat.data[0] * quat.data[2]);
  mat.data[3] = 0.0f;

  mat.data[4] = 2.0f *
	(quat.data[1] * quat.data[2] + quat.data[0] * quat.data[3]);
  mat.data[5] = 1.0f - 2.0f *
	(quat.data[1] * quat.data[1] + quat.data[3] * quat.data[3]);
  mat.data[6] = 2.0f *
	(quat.data[2] * quat.data[3] - quat.data[0] * quat.data[1]);
  mat.data[7] = 0.0f;

  mat.data[8] = 2.0f *
	(quat.data[1] * quat.data[3] - quat.data[0] * quat.data[2]);
  mat.data[9] = 2.0f *
	(quat.data[2] * quat.data[3] + quat.data[0] * quat.data[1]);
  mat.data[10] = 1.0f - 2.0f *
	(quat.data[1] * quat.data[1] + quat.data[2] * quat.data[2]);
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
    quat.data[1] = (mat.data[9] - mat.data[6]) / scale;
    quat.data[2] = (mat.data[2] - mat.data[8]) / scale;
    quat.data[3] = (mat.data[4] - mat.data[1]) / scale;
    quat.data[0] = 0.25 * scale;
    }
  else {
    if(mat.data[0] > mat.data[5] && mat.data[0] > mat.data[10]) {
      scale  = sqrt(1.0f + mat.data[0] - mat.data[5] - mat.data[10]) * 2.0f;
      quat.data[1] = 0.25f * scale;
      quat.data[2] = (mat.data[1] + mat.data[4]) / scale;
      quat.data[3] = (mat.data[2] + mat.data[8]) / scale;
      quat.data[0] = (mat.data[9] - mat.data[6]) / scale;
      }
    else if(mat.data[5] > mat.data[10]) {
      scale  = sqrt(1.0f + mat.data[5] - mat.data[0] - mat.data[10]) * 2.0f;
      quat.data[1] = (mat.data[1] + mat.data[4]) / scale;
      quat.data[2] = 0.25f * scale;
      quat.data[3] = (mat.data[6] + mat.data[9]) / scale;
      quat.data[0] = (mat.data[2] - mat.data[8]) / scale;
      }
    else {
      scale  = sqrt(1.0f + mat.data[10] - mat.data[0] - mat.data[5]) * 2.0f;
      quat.data[1] = (mat.data[2] + mat.data[8]) / scale;
      quat.data[2] = (mat.data[6] + mat.data[9]) / scale;
      quat.data[3] = 0.25f * scale;
      quat.data[0] = (mat.data[4] - mat.data[1]) / scale;
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
  return SimpleModel::LoadModel(filename, defskin);
  }
