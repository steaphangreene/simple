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

#include <GL/gl.h>

#include "sg_dndboxes.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_events.h"

SG_DNDBoxes::SG_DNDBoxes(int xsz, int ysz)
	: SG_Compound(xsz, ysz, 0.0, 0.0) {
  }

SG_DNDBoxes::~SG_DNDBoxes() {
  }

bool SG_DNDBoxes::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    }
  return 0; // Silence children doing other things
  }

//  bool SG_DNDBoxes::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_DNDBoxes::Default_Mouse_Cursor = NULL;
