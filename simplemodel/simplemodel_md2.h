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

#ifndef SIMPLEMODEL_MD2_H
#define SIMPLEMODEL_MD2_H

#include "simplemodel.h"

class SimpleModel_MD2 : public SimpleModel {
 public:
  SimpleModel_MD2(const string &filenm, const string &modelnm,
                  const string &defskin = "default");
  virtual ~SimpleModel_MD2();

  virtual bool Load(const string &filenm, const string &modelnm,
                    const string &defskin = "default");

  virtual bool MoveToTag(Uint32 tag, Uint32 cur_time,
                         const vector<int> &anim = vector<int>(),
                         const vector<Uint32> &start_time = vector<Uint32>(),
                         Uint32 anim_offset = 0) const;

  virtual const vector<string> &GetSkinList();
  virtual void AddSkin(const string &skinnm);

 protected:
  string modelname;
  vector<string> skins;
  vector<vector<Vector3> > verts;
  vector<vector<Vector3> > norms;
  vector<vector<Vector2> > tex_coords;

  struct GLVertex {
    float tex_x, tex_y;
    Uint32 vindex;
  };

  struct GLCommand {
    bool strip;
    vector<GLVertex> verts;
  };

  vector<GLCommand> glcomms;

  virtual bool RenderSelf(Uint32 cur_time,
                          const vector<int> &anim = vector<int>(),
                          const vector<Uint32> &start_time = vector<Uint32>(),
                          Uint32 anim_offset = 0) const;

  Uint32 NormalizeFrame(int anim, Uint32 frame) const;
  Uint32 CalcBaseFrame(Uint32 cur_time, int anim, Uint32 start_time,
                       float &offset) const;
};

#endif  // SIMPLEMODEL_MD2_H
