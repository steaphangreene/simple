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

#ifndef	SIMPLEMODEL_3DS_H
#define	SIMPLEMODEL_3DS_H

#include "simplemodel.h"

class SimpleModel_3DS : public SimpleModel {
public:
  SimpleModel_3DS();
  SimpleModel_3DS(const string &filenm);
  virtual ~SimpleModel_3DS();

  virtual bool Load(const string &filenm);

  virtual bool Render(Uint32 cur_time);

  virtual void SetAnimation(int part);

  virtual int GetAnimation();

protected:
  };

#endif	//SIMPLEMODEL_3DS_H
