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

#ifndef SIMPLEMODEL_WEDGE_H
#define SIMPLEMODEL_WEDGE_H

#include "simplemodel.h"
#include "simplemodel_md3.h"

class SimpleModel_Wedge : public SimpleModel {
 public:
  SimpleModel_Wedge();
  SimpleModel_Wedge(const string &filenm);
  virtual ~SimpleModel_Wedge();

  virtual bool Load(const string &filenm);

 protected:
  virtual bool RenderSelf(Uint32 cur_time,
                          const vector<int> &anim = vector<int>(),
                          const vector<Uint32> &start_time = vector<Uint32>(),
                          Uint32 anim_offset = 0) const;
};

#endif  // SIMPLEMODEL_WEDGE_H
