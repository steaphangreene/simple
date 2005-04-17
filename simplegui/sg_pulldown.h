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

#ifndef SG_PULLDOWN_H
#define SG_PULLDOWN_H

#include "sg_compound.h"
#include "sg_texture.h"
#include "sg_colors.h"

class SG_Button;
class SG_Menu;

class SG_PullDown : public SG_Compound {
public:
  SG_PullDown(const string &mes, const vector<string> &itms,
	SG_Texture tex = SG_COL_RAISED, SG_Texture dis_tex = SG_COL_LOW,
	SG_Texture click_tex = SG_COL_LOW);
  virtual ~SG_PullDown();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  SG_Button *but;
  SG_Menu *menu;
  };

#endif // SG_PULLDOWN_H
