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

#include "SDL_opengl.h"

#include "simpleconnect.h"
#include "../simplegui/sg_events.h"
#include "../simplegui/sg_stickybutton.h"

SimpleConnect::SimpleConnect(const vector<SC_SlotType> &slts)
	: SG_Compound(8, slts.size() + 1, 0.1, 0.1) {
  slots = slts;
  network = false;

  background = new SG_Panel(SG_COL_FG);
  for(unsigned int slot = 0; slot < slots.size(); ++slot) {
    SG_StickyButton *meb = new SG_StickyButton("Me");
    SG_TextArea *name = NULL;
    if(slot == 0) {
      meb->TurnOn();
      name = new SG_TextArea("Local Player");
      }
    else {
      name = new SG_TextArea("AI Player");
      }
    AddWidget(name, 4, slot+1, 4, 1);
    AddWidget(meb, 0, slot+1, 1, 1);
    }
  SG_Widget *labelb =
	new SG_TextArea("SimpleConnect", SG_COL_LOW);
  AddWidget(labelb, 0, 0, 6, 1);
  okb = new SG_Button("Ok", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(okb, 7, 0, 1, 1);
  }

SimpleConnect::~SimpleConnect() {
  }

void SimpleConnect::EnableNetwork(const string &tag) {
  nettag = tag;
  network = true;
  }

bool SimpleConnect::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(okb)) {
      event->user.code = SG_EVENT_OK;
      event->user.data1 = (void*)this;
      event->user.data2 = NULL;
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

//  bool SimpleConnect::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConnect::Default_Mouse_Cursor = NULL;
