// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#ifndef SG_MULTITAB_H
#define SG_MULTITAB_H

#include <vector>
using namespace std;

#include "sg_compound.h"
#include "sg_tabs.h"

class SG_Alignment;

class SG_MultiTab : public SG_Compound {
public:
  SG_MultiTab(vector<string> items, vector<SG_Alignment *> areas,
	int tinvpro = 8, 
	SimpleTexture ttex = SG_COL_RAISED, SimpleTexture dis_ttex = SG_COL_LOW,
	SimpleTexture click_ttex = SG_COL_LOW, SimpleTexture down_ttex = SG_COL_HIGH);
  virtual ~SG_MultiTab();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  void Set(int);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  vector<SG_Alignment *> subscreens;
  };

#endif // SG_MULTITAB_H
