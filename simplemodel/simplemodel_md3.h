// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
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

struct Md3AnimationData {
  long start;
  long end;
  long loop; // Number of frames that should be looped, I ignore this (for now)
  long fps;
  };

struct Md3Tag {
  char name[64];
  Quaternion pos;
  };

struct Md3Face {
  long vertices[3];
  };

struct Md3TexCoord {
  float coord[2];
  };

struct Md3Triangle {
  float vertex[3];
  unsigned char normal[2];	// FIXME: Don't know how these work yet.
  };

struct Md3Mesh {
  char name[68];
  vector<Md3Face> faces;
  vector<Md3TexCoord> coords;
  vector<Md3Triangle> triangles;
  GLuint texture;
  };

class SimpleModel_Md3 : public SimpleModel {
public:
  SimpleModel_Md3(const string &filenm,
	const string &modelnm, const string &skinnm = "");
  virtual ~SimpleModel_Md3();

  virtual bool Load(const string &filenm,
	const string &filenm, const string &skinnm = "");

  virtual bool Render(Uint32 cur_time);
  bool MoveToTag(unsigned long tag);

  unsigned long TagNameToIndex(const string &tagname);
  bool MoveToTag(const string &tagname) {
    return MoveToTag(TagNameToIndex(tagname));
    }

  virtual void SetAnimation(int anim);

  virtual int GetAnimation();

  int AddAnimation(int start, int end, int loop, int fps);

protected:
  SimpleModel_Md3();
  string modelname, skinname;

  unsigned long num_tags;	// Number of tags PER FRAME
  vector<Md3Tag> pTags;		// Holds ALL tags for ALL frames
  vector<Md3Mesh> meshes;
  vector<Md3AnimationData> animations;

	//Temporary - these should not be here for static models
  int current_anim;
  int current_frame;
  Uint32 last_time;
  };

#endif	//SIMPLEMODEL_MD3_H
