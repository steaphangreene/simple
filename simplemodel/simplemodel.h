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

#ifndef	SIMPLEMODEL_H
#define	SIMPLEMODEL_H

#include <string>
#include <vector>
using namespace std;

#include "SDL.h"
#include "SDL_opengl.h"

#include "../simpletexture/simpletexture.h"

class SimpleModel {
public:
  SimpleModel();
  virtual ~SimpleModel();

  virtual bool Load(const string &filenm);

  virtual bool Render(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>()) const;

  virtual const vector<string> &GetSkinList();
  virtual void AddSkin(const string &skinnm) {};

  //Configuration Functions
  static void AddSourceFile(const string &in);
  static void ClearSourceFiles();
  static const vector<string> GetSourceFiles();

  //Exported Utility Functions
  static SimpleModel *LoadModel(const string &filename, const string &defskin = "");

protected:
  string filename;
  vector<SimpleTexture *> texture;

  static vector<string> source_files;

  struct Matrix4x4 {
    float data[16];
    };

  struct Quaternion {
    float data[4];
    };

  //Conversions
  static void QuaternionToMatrix4x4(Matrix4x4 &mat, const Quaternion &quat);
  static void Matrix4x4ToQuaternion(Quaternion &quat, const Matrix4x4 &mat);

  //Math
  static void Multiply(Matrix4x4 &res,
	const Matrix4x4 &m1, const Matrix4x4 &m2);
  static void MultiplyTranspose(Matrix4x4 &res,	//Temporary!
	const Matrix4x4 &m1, const Matrix4x4 &m2);

  //Interpolations
  static void SLERP(Matrix4x4 &res,
	const Matrix4x4 &m1, const Matrix4x4 &m2, const float t);
  static void SLERP(Quaternion &res,
	const Quaternion &q1, const Quaternion &q2, const float t);
  };

SimpleModel *SM_LoadModel(const string &filename, const string &defskin = "");

#endif	//SIMPLEMODEL_H
