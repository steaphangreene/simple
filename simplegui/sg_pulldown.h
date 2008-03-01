// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "sg_multitext.h"
#include "simpletexture.h"
#include "sg_colors.h"

class SG_Button;
class SG_Menu;

class SG_PullDown : public SG_Compound, public SG_MultiText{
public:
  SG_PullDown(const string &mes, const vector<string> &itms,
	SimpleTexture tex = SG_COL_RAISED, SimpleTexture dis_tex = SG_COL_LOW,
	SimpleTexture click_tex = SG_COL_LOW);
  virtual ~SG_PullDown();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

  virtual const string &Item(int opt);
  virtual void SetItems(const vector<string> &itms);
  virtual int NumItems();

  void SetID(int id);
  int ID();
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  SG_Button *but;
  SG_Menu *menu;
  };

#endif // SG_PULLDOWN_H
