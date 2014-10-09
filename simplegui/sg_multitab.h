// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
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

class SG_MultiTab : public SG_Compound, public SG_MultiText {
 public:
  SG_MultiTab(const vector<string> &items = vector<string>(),
              const vector<SG_Alignment *> &areas = vector<SG_Alignment *>(),
              int tinvpro = 8, SimpleTexture ttex = SG_COL_RAISED,
              SimpleTexture dis_ttex = SG_COL_LOW,
              SimpleTexture click_ttex = SG_COL_LOW,
              SimpleTexture down_ttex = SG_COL_HIGH);
  virtual ~SG_MultiTab();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  virtual const string &Item(int opt);
  virtual void SetItems(const vector<string> &items);
  virtual int NumItems();
  void SetArea(int page, SG_Alignment *area);
  void SetAreas(const vector<SG_Alignment *> &areas);
  void Set(int);

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;
  SG_Tabs *tabs;
  vector<SG_Alignment *> subscreens;
};

#endif  // SG_MULTITAB_H
