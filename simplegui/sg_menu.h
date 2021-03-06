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

#ifndef SG_MENU_H
#define SG_MENU_H

#include <vector>
using namespace std;

#include "sg_button.h"
#include "sg_multitext.h"

class SG_Menu : public SG_Button, SG_MultiText {
 public:
  SG_Menu(const vector<string> &itms, SimpleTexture tex = SG_COL_RAISED,
          SimpleTexture dis_tex = SG_COL_LOW,
          SimpleTexture click_tex = SG_COL_LOW);
  virtual ~SG_Menu();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual int HandleEvent(SDL_Event *event, float x, float y);

  void SetID(int id);
  int ID() { return menu_id; };

  virtual const string &Item(int opt);
  virtual void SetItems(const vector<string> &itms);
  virtual int NumItems();

 protected:
  virtual bool RenderSelf(unsigned long cur_time);

  //  static GL_MODEL Default_Mouse_Cursor;
  vector<string> items;
  int menu_id;
};

#endif  // SG_MENU_H
