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

#ifndef	SIMPLEMODEL_MD3_H
#define	SIMPLEMODEL_MD3_H

#include "simplemodel.h"

struct Quaternion {
  float data[16];
  };

struct MD3AnimationData {
  Sint32 start;
  Sint32 end;
  Sint32 loop; // Number of frames that should be looped, I ignore this (for now)
  Sint32 fps;
  };

struct MD3Tag {
  Sint8 name[64];
  Quaternion pos;
  };

struct MD3Face {
  Sint32 vertices[3];
  };

struct MD3TexCoord {
  float coord[2];
  };

struct MD3Triangle {
  float vertex[3];
  Uint8 normal[2];	// FIXME: Don't know how these work yet.
  };

struct MD3Mesh {
  Sint8 name[68];
  vector<MD3Face> faces;
  vector<MD3TexCoord> coords;
  vector<MD3Triangle> triangles;
  SimpleTexture *texture;
  };

class SimpleModel_MD3 : public SimpleModel {
public:
  SimpleModel_MD3(const string &filenm,
	const string &modelnm, const string &defskin = "default");
  virtual ~SimpleModel_MD3();

  virtual bool Load(const string &filenm,
	const string &filenm, const string &defskin = "default");

  virtual bool Render(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>()) const;
  bool MoveToTag(unsigned long tag, Uint32 cur_time,
        const vector<int> &anim = vector<int>(),
        const vector<Uint32> &start_time = vector<Uint32>()) const;

  unsigned long TagNameToIndex(const string &tagname) const;
  bool MoveToTag(const string &tagname, Uint32 cur_time,
        const vector<int> &anim = vector<int>(),
        const vector<Uint32> &start_time = vector<Uint32>()) const {
    return MoveToTag(TagNameToIndex(tagname), cur_time, anim, start_time);
    }

  virtual const vector<string> &GetSkinList();
  virtual void AddSkin(const string &skinnm);

  int AddAnimation(int start, int end, int loop, int fps);

protected:
  SimpleModel_MD3();
  string modelname, skinname;

  int CalcFrame(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time) const;

  Uint32 num_tags;	// Number of tags PER FRAME
  vector<MD3Tag> pTags;		// Holds ALL tags for ALL frames
  vector<MD3Mesh> meshes;
  vector<MD3AnimationData> animations;
  vector<string> skins;
  };

#endif	//SIMPLEMODEL_MD3_H
