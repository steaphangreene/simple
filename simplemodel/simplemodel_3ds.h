// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  3DS support written by Noah Schwartz with chunk parsing code
//  based on Damiano Vitulli's 3DS loading tutorial at
//  http://spacesimulator.net/tut4_3dsloader.html
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

#ifndef	SIMPLEMODEL_3DS_H
#define	SIMPLEMODEL_3DS_H

#include "simplemodel.h"

#include <sys/stat.h>

struct MAXVertex{
    GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct MAXPolygon {
    Uint16 v1;
	Uint16 v2;
	Uint16 v3;
};

struct MAXTexCoord {
    GLfloat u;
	GLfloat v;
};

struct MAXMesh{
	char name[20];

    vector<MAXVertex> vertex;
    vector<MAXPolygon> polygon;
    vector<MAXTexCoord> tex_coord;
	bool has_texture;
    SimpleTexture *texture;
};

class SimpleModel_3DS : public SimpleModel {
public:
  SimpleModel_3DS();
  SimpleModel_3DS(const string &filenm, const string &texnm = "");
  virtual ~SimpleModel_3DS();

  virtual bool Load(const string &filenm, const string &texnm = "");

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;

  GLuint display_list;
  vector<MAXMesh> mesh;
  };

#endif	//SIMPLEMODEL_3DS_H
