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

#ifndef	SIMPLEMODEL_Q3DIR_H
#define	SIMPLEMODEL_Q3DIR_H

#include "simplemodel.h"
#include "simplemodel_md3.h"

class SimpleModel_Q3Dir : public SimpleModel {
public:
  SimpleModel_Q3Dir(const string &filenm);
  virtual ~SimpleModel_Q3Dir();

  virtual bool Load(const string &filenm);

  virtual bool Render(Uint32 cur_time,
	const vector<int> &anim = vector<int>(),
	const vector<Uint32> &start_time = vector<Uint32>());

  virtual void SetAnimation(int part);

  virtual int GetAnimation();

  void SetWeapon(SimpleModel_MD3 *weap);

protected:
  bool LoadCFG(const string &filenm);

  SimpleModel_Q3Dir();
  SimpleModel_MD3 *head, *torso, *legs, *weapon;
  };

#endif	//SIMPLEMODEL_Q3DIR_H
