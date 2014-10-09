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

#ifndef SIMPLEMODEL_H
#define SIMPLEMODEL_H

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "SDL.h"
#include "SDL_opengl.h"

#include "simpletexture.h"

class SimpleModel {
 public:
  SimpleModel();
  virtual ~SimpleModel();

  const map<string, int> GetAnimations() {
    return animations;
  };
  int LookUpAnimation(const string &anim) const;
  virtual bool LoadAnimation(const string &filenm) {
    return false;
  };

  const map<string, Uint32> GetTags() {
    return tags;
  };

  bool Render(Uint32 cur_time, const vector<int> &anim = vector<int>(),
              const vector<Uint32> &start_time = vector<Uint32>(),
              Uint32 anim_offset = 0, float fac = 0.0, float elv = 0.0) const;

  virtual bool MoveToTag(Uint32 tag, Uint32 cur_time,
                         const vector<int> &anim = vector<int>(),
                         const vector<Uint32> &start_time = vector<Uint32>(),
                         Uint32 anim_offset = 0) const;
  void AttachSubmodel(Uint32 tag, SimpleModel *submodel);
  void DetachSubmodel(const SimpleModel *submodel);
  void DetachSubmodel(Uint32 tag);
  void SetAnimOffset(Uint32 tag, Uint32 offset);

  Uint32 TagNameToIndex(const string &tagname) const;
  bool MoveToTag(const string &tagname, Uint32 cur_time,
                 const vector<int> &anim = vector<int>(),
                 const vector<Uint32> &start_time = vector<Uint32>(),
                 Uint32 anim_offset = 0) const;
  void AttachSubmodel(const string &tag, SimpleModel *submodel);
  void DetachSubmodel(const string &tag);
  void SetAnimOffset(const string &tag, Uint32 offset);

  virtual bool Load(const string &filenm,
                    const vector<string> &skin = vector<string>());

  virtual const vector<string> &GetSkinList();
  virtual void AddSkin(const string &skinnm) {};

  void DisableMaterial(int matnum) {
    disabled_materials.insert(matnum);
  };
  void EnableMaterial(int matnum) {
    disabled_materials.erase(matnum);
  };
  void ToggleMaterial(int matnum) {
    if (MaterialDisabled(matnum)) {
      EnableMaterial(matnum);
    } else {
      DisableMaterial(matnum);
    }
  };
  bool MaterialDisabled(int matnum) const {
    return (disabled_materials.count(matnum) > 0);
  };

  set<int> disabled_materials;

  // Configuration Functions
  static void AddSourceFile(const string &in);
  static void ClearSourceFiles();
  static const vector<string> GetSourceFiles();

  // Exported Utility Functions
  static SimpleModel *LoadModel(const string &filename,
                                const string &defskin = "");
  static SimpleModel *LoadModel(const string &filename,
                                const vector<string> &skins);

  // Exported Sub-Classes, and their Support Function
  struct Vector2 {
    float data[2];
  };

  struct Vector3 {
    float data[3];
  };

  struct Vector4 {
    float data[4];
  };

  struct Matrix4x4 {
    float data[16];
  };

  static Matrix4x4 identity4x4;

  struct Quaternion {
    float w, x, y, z;
  };

  // Conversions
  static void QuaternionToMatrix4x4(Matrix4x4 &mat, const Quaternion quat);
  static void Matrix4x4ToQuaternion(Quaternion &quat, const Matrix4x4 mat);

  // Math
  template <class Tp>
  static void Add(Tp &res, const Tp m1, const Tp m2) {
    for (unsigned int n = 0; n < sizeof(res.data) / sizeof(res.data[0]); ++n) {
      res.data[n] = m1.data[n] + m2.data[n];
    }
  }
  template <class Tp>
  static void Multiply(Tp &res, const Tp mat, float scale) {
    for (unsigned int n = 0; n < sizeof(res.data) / sizeof(res.data[0]); ++n) {
      res.data[n] = mat.data[n] * scale;
    }
  }
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3, const Matrix4x4 m4);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3, const Matrix4x4 m4,
                       const Matrix4x4 m5);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3, const Matrix4x4 m4,
                       const Matrix4x4 m5, const Matrix4x4 m6);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3, const Matrix4x4 m4,
                       const Matrix4x4 m5, const Matrix4x4 m6,
                       const Matrix4x4 m7);
  static void Multiply(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                       const Matrix4x4 m3, const Matrix4x4 m4,
                       const Matrix4x4 m5, const Matrix4x4 m6,
                       const Matrix4x4 m7, const Matrix4x4 m8);

  static void MatrixTransform(Vector3 &v, const Matrix4x4 &rot);
  static void MatrixTransform(float &x, float &y, float &z,
                              const Matrix4x4 &rot);
  static void Normalize(Matrix4x4 &res, const Matrix4x4 quat);

  // Interpolations
  template <class Tp>
  static void LERP(Tp &res, const Tp m1, const Tp m2, float t) {
    for (unsigned int n = 0; n < sizeof(res.data) / sizeof(res.data[0]); ++n) {
      res.data[n] = m1.data[n] * (1.0 - t) + m2.data[n] * t;
    }
  }
  static void SLERP(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                    const float t);
  static void SLERP(Quaternion &res, const Quaternion q1, const Quaternion q2,
                    const float t);
  static void BERP(Matrix4x4 &res, const Matrix4x4 m1, const Matrix4x4 m2,
                   const float t, const float *const bez_x,
                   const float *const bez_y, const float *const bez_z,
                   const float *const bez_r);
  static void BERP(Vector3 &res, const Vector3 v1, const Vector3 v2,
                   const float t, const float *const bez_x,
                   const float *const bez_y, const float *const bez_z);
  static void BERP(Quaternion &res, const Quaternion q1, const Quaternion q2,
                   const float t, const float *const bez_r);

  static void Multiply(Quaternion &res, Quaternion q1, Quaternion q2);
  static void QuaternionRotate(float &x, float &y, float &z,
                               const Quaternion &rot);
  static void QuaternionRotate(Vector3 &v, const Quaternion &rot);
  static void Normalize(Quaternion &res, const Quaternion quat);

 protected:
  virtual bool RenderSelf(Uint32 cur_time,
                          const vector<int> &anim = vector<int>(),
                          const vector<Uint32> &start_time = vector<Uint32>(),
                          Uint32 anim_offset = 0) const;

  string filename;
  vector<SimpleTexture *> texture;
  map<string, int> animations;
  map<string, Uint32> tags;
  map<Uint32, SimpleModel *> submodels;
  map<Uint32, Uint32> tag_anim_offsets;
  set<Uint32> target_models;

  static vector<string> source_files;

  static bool gl_ext_detected;
  static void SetupGLEXT();

  typedef void(APIENTRY *glGenVBO)(GLsizei, GLuint *);
  typedef void(APIENTRY *glBndVBO)(GLenum, GLuint);
  typedef void(APIENTRY *glBufVBO)(GLenum, GLsizeiptr, const GLvoid *, GLenum);
  typedef void(APIENTRY *glDelVBO)(GLsizei, const GLuint *);
  typedef GLboolean(APIENTRY *glIsAVBO)(GLuint);

  static glGenVBO glGenBuffersARB;
  static glBndVBO glBindBufferARB;
  static glBufVBO glBufferDataARB;
  static glDelVBO glDeleteBuffersARB;
  static glIsAVBO glIsBufferARB;

  typedef void(APIENTRY *glActTEX)(GLenum);
  typedef void(APIENTRY *glMTXCRD2f)(GLenum, GLfloat, GLfloat);

  static glActTEX glActiveTextureARB;
  static glMTXCRD2f glMultiTexCoord2fARB;
};

SimpleModel *SM_LoadModel(const string &filename, const string &defskin = "");
SimpleModel *SM_LoadModel(const string &filename, const vector<string> &skins);

#endif  // SIMPLEMODEL_H
