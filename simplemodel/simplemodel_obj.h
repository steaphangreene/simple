// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef	SIMPLEMODEL_OBJ_H
#define	SIMPLEMODEL_OBJ_H

#include "simplemodel.h"

class SimpleModel_OBJ : public SimpleModel {
public:
  SimpleModel_OBJ();
  SimpleModel_OBJ(const string &filenm, const string &texnm = "");
  virtual ~SimpleModel_OBJ();

  virtual bool Load(const string &filenm, const string &texnm = "");

protected:
  virtual bool RenderSelf(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>(),
	Uint32 anim_offset = 0) const;
  GLuint vertices;
  GLenum format;
  int num_verts;
  };

#endif	//SIMPLEMODEL_OBJ_H
