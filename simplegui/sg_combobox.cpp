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

#include <SDL/SDL_opengl.h>

#include "sg_combobox.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_events.h"

SG_ComboBox::SG_ComboBox(vector<string> options, int binvpro,
        SG_Texture btex, SG_Texture btex_dis, SG_Texture btex_click,
        SG_Texture ttex, SG_Texture ttex_dis)
	: SG_Compound(binvpro, 1, 0.0, 0.0) {
  opb = new SG_Button("v", btex, btex_dis, btex_click);
  AddWidget(opb, binvpro-1, 0, 1, 1);
  SG_Widget *labelb =
	new SG_TextArea(options[0], ttex, ttex_dis);
  AddWidget(labelb, 0, 0, binvpro-1, 1);
  }

SG_ComboBox::~SG_ComboBox() {
  }

bool SG_ComboBox::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    //Unimplemented
    }
  return 0; // Silence children doing other things
  }

//  bool SG_ComboBox::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_ComboBox::Default_Mouse_Cursor = NULL;
