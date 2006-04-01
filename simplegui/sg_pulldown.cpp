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

#include "SDL_opengl.h"

#include "sg_pulldown.h"
#include "sg_globals.h"
#include "sg_events.h"
#include "sg_button.h"
#include "sg_menu.h"

SG_PullDown::SG_PullDown(const string &mes, const vector<string> &itms,
	SimpleTexture tex, SimpleTexture dis_tex, SimpleTexture click_tex)
	: SG_Compound(1, 1, 0.0, 0.0) {
  but = new SG_Button(mes, tex, dis_tex, click_tex);
  AddWidget(but, 0, 0);

  menu = new SG_Menu(itms);
  AddWidget(menu, 0, 0);
  wgeom[1].ypos = 1;
  wgeom[1].ysize = 1;
  }

SG_PullDown::~SG_PullDown() {
  }

bool SG_PullDown::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    current_sg->SetCurrentWidget(menu);
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  if(current_sg->CurrentWidget() != (SG_Widget*)menu) {
    if(but->State() == 2) but->SetState(0);
    }
  if(event->user.data1 == (void*)(SG_MultiText*)menu) {
    event->user.data1 = (void*)(SG_MultiText*)this;
    return 1;	// Pass menu events through as my events
    }
  return 0; // Silence children doing other things
  }

void SG_PullDown::SetID(int id) {
  menu->SetID(id);
  }

int SG_PullDown::ID() {
  return menu->ID();
  }

const string &SG_PullDown::Item(int opt) {
  return menu->Item(opt);
  }

void SG_PullDown::SetItems(const vector<string> &itms) {
  menu->SetItems(itms);
  }

//  bool SG_PullDown::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_PullDown::Default_Mouse_Cursor = NULL;
