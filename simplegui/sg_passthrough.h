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

#ifndef SG_PASSTHROUGH_H
#define SG_PASSTHROUGH_H

#include "sg_alignment.h"
#include "sg_menu.h"

class SG_PassThrough : public SG_Alignment {
public:
  SG_PassThrough(int lact, int mact, int ract);
  virtual ~SG_PassThrough();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool Render(unsigned long cur_time);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetBehavior(int lact, int mact, int ract);
  void SetMenu(int but, const vector<string> itms);
  
protected:
  int button_action[3];		//Auto-Handling of the three main buttons
  SG_Menu *button_menu[3];	//Click Menus for the three main buttons
  int cur_action, cur_button;
  float act_x, act_y;
  float cur_x, cur_y;

  void CalcGeometry();

//  static GL_MODEL Default_Mouse_Cursor;
  };

#define SG_PT_IGNORE	0
#define SG_PT_BLOCK	1
#define SG_PT_CLICK	2
#define SG_PT_BOX	3
#define SG_PT_MENU	4

#endif // SG_PASSTHROUGH_H
