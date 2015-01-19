// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SG_COMBOBOX_H
#define SG_COMBOBOX_H

#include <vector>
using namespace std;

#include "sg_compound.h"
#include "simpletexture.h"
#include "sg_colors.h"
#include "sg_text.h"
#include "sg_multitext.h"

class SG_StickyButton;
class SG_TextArea;
class SG_Menu;

class SG_ComboBox : public SG_Compound, public SG_Text, public SG_MultiText {
 public:
  SG_ComboBox(
      const vector<string> &options, bool edit = false,
      SimpleTexture btex = SG_COL_RAISED, SimpleTexture btex_dis = SG_COL_LOW,
      SimpleTexture btex_click = SG_COL_LOW, SimpleTexture ttex = SG_COL_FG,
      SimpleTexture ttex_dis = SG_COL_BG, SimpleTexture ttex_fg = SG_COL_HIGH,
      SimpleTexture mtex = SG_COL_BG, SimpleTexture mtex_dis = SG_COL_LOW,
      SimpleTexture mtex_sel = SG_COL_RAISED);
  virtual ~SG_ComboBox();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  virtual const string &Text();
  virtual void SetText(const string &txt);
  virtual const string &Item(int opt);
  virtual void SetItems(const vector<string> &itms);
  virtual int NumItems();

  void Set(int);

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;
  SG_TextArea *text;
  SG_StickyButton *opb;
  SG_Menu *menu;
};

#endif  // SG_COMBOBOX_H
