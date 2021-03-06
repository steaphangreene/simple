// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SIMPLEMODEL_MD3_H
#define SIMPLEMODEL_MD3_H

#include "simplemodel.h"

class SimpleModel_MD3 : public SimpleModel {
 public:
  SimpleModel_MD3(const string &filenm, const string &modelnm,
                  const string &defskin = "default");
  virtual ~SimpleModel_MD3();

  virtual bool Load(const string &filenm, const string &modelnm,
                    const string &defskin = "default");

  virtual bool MoveToTag(Uint32 tag, Uint32 cur_time,
                         const vector<int> &anim = vector<int>(),
                         const vector<Uint32> &start_time = vector<Uint32>(),
                         Uint32 anim_offset = 0) const;

  // WTF???  Why the hell won't GCC figure this out by itself???
  // SimpleModel_MD3 is publically derived from SimpleModel!!!!
  // And this is a public member function of SimpleModel!!!!
  bool MoveToTag(const string &tag, Uint32 cur_time,
                 const vector<int> &anim = vector<int>(),
                 const vector<Uint32> &start_time = vector<Uint32>()) const {
    return SimpleModel::MoveToTag(tag, cur_time, anim, start_time);
  }

  virtual const vector<string> &GetSkinList();
  virtual void AddSkin(const string &skinnm);

  int AddAnimation(int start, int end, int loop, int fps);

 protected:
  virtual bool RenderSelf(Uint32 cur_time,
                          const vector<int> &anim = vector<int>(),
                          const vector<Uint32> &start_time = vector<Uint32>(),
                          Uint32 anim_offset = 0) const;

  SimpleModel_MD3();
  string modelname, skinname;

  int NormalizeFrame(int anim, int frame) const;
  int CalcBaseFrame(Uint32 cur_time, int anim, Uint32 start_time,
                    float &offset) const;

  struct MD3AnimationData {
    Sint32 start;
    Sint32 end;
    Sint32 loop;  // Number of frames that should be looped, I ignore this (for
                  // now)
    Sint32 fps;
  };

  struct MD3Tag {
    Sint8 name[64];
    Matrix4x4 pos;
  };

  struct MD3Face {
    Sint32 vertices[3];
  };

  struct MD3TexCoord {
    float coord[2];
  };

  struct MD3Triangle {
    float vertex[3];
    float normal[3];
  };

  struct MD3Mesh {
    Sint8 name[68];
    vector<MD3Face> faces;
    vector<MD3TexCoord> coords;
    vector<MD3Triangle> triangles;
    SimpleTexture *texture;
  };

  Uint32 num_tags;       // Number of tags PER FRAME
  vector<MD3Tag> pTags;  // Holds ALL tags for ALL frames
  vector<MD3Mesh> meshes;
  vector<MD3AnimationData> anim_data;
  vector<string> skins;
};

#endif  // SIMPLEMODEL_MD3_H
