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

#include "simpleconfig.h"
#include "../simplegui/sg_events.h"
#include "../simplegui/sg_globals.h"
#include "../simplegui/sg_button.h"
#include "../simplegui/sg_dragable.h"
#include "../simplegui/sg_editable.h"

SimpleConfig::SimpleConfig()
	: SG_MultiTab(vector<string>(4), vector<SG_Alignment*>(4)) {
  }

SimpleConfig::~SimpleConfig() {
  }

bool SimpleConfig::Render(unsigned long cur_time) {
  return SG_Compound::Render(cur_time);
  }

bool SimpleConfig::ChildEvent(SDL_Event *event) {
  //FIXME: Implement!
  return 0; // Silence children doing other things
  }

//  bool SimpleConfig::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConfig::Default_Mouse_Cursor = NULL;
