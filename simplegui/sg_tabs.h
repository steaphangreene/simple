// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef SG_TABS_H
#define SG_TABS_H

#include <string>
#include <vector>
using namespace std;

#include "sg_compound.h"
#include "sg_multitext.h"
#include "../simpletexture/simpletexture.h"
#include "sg_colors.h"

#ifndef SG_AUTOSIZE
#define SG_AUTOSIZE	-1
#endif

class SG_StickyButton;

class SG_Tabs : public SG_Compound, public SG_MultiText {
public:		// Note: only either x OR y can be SG_AUTOSIZE, NOT BOTH!
  SG_Tabs(const vector<string> &items = vector<string>(),
	int x = SG_AUTOSIZE, int y = SG_AUTOSIZE,
	SimpleTexture tex = SG_COL_RAISED, SimpleTexture dis_tex = SG_COL_LOW,
	SimpleTexture click_tex = SG_COL_LOW, SimpleTexture down_tex = SG_COL_HIGH);
  virtual ~SG_Tabs();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  int Which() { return cur_on; }

  virtual const string &Item(int opt);
  virtual void SetItems(const vector<string> &items);
  virtual int NumItems();

  void Set(int which);
  void Next(bool wrap = false);
  void Prev(bool wrap = false);
  void Right(bool wrap = false);
  void Left(bool wrap = false);
  void Up(bool wrap = false);
  void Down(bool wrap = false);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  int cur_on; //Which widget is currently on
  int fixed_x, fixed_y;
  };

#endif // SG_TABS_H
