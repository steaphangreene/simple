// *************************************************************************
//  This file is part of the SimpleConfig Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleConfig is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleConfig is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConfig (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL_opengl.h"

#include "simpleconfig.h"
#include "../simplegui/sg_events.h"
#include "../simplegui/sg_globals.h"
#include "../simplegui/sg_button.h"
#include "../simplegui/sg_dragable.h"
#include "../simplegui/sg_textarea.h"

SimpleConfig::SimpleConfig(const vector<string> &other_tabs,
        const vector<SG_Alignment*> &other_screens) {
  vector<string> items(4 + other_tabs.size());
  vector<SG_Alignment*> screens(4 + other_tabs.size());
  items[0] = "Video";
  items[1] = "Audio";
  items[2] = "Mouse";
  items[3] = "Keyboard";
  screens[0] = new SG_Alignment;
  ((SG_Alignment*)(screens[0]))->SetBorder(0.25, 0.25);
  ((SG_Alignment*)(screens[0]))->AddWidget(new SG_TextArea("Video Settings"));
  screens[1] = new SG_Alignment;
  screens[2] = new SG_Alignment;
  screens[3] = new SG_Alignment;
  copy(other_tabs.begin(), other_tabs.end(), items.begin()+4);
  copy(other_screens.begin(), other_screens.end(), screens.begin()+4);
  SetItems(items, screens);
  }

SimpleConfig::~SimpleConfig() {
  }

bool SimpleConfig::Render(unsigned long cur_time) {
  return SG_MultiTab::Render(cur_time);
  }

bool SimpleConfig::ChildEvent(SDL_Event *event) {
  return SG_MultiTab::ChildEvent(event);
  }

//  bool SimpleConfig::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConfig::Default_Mouse_Cursor = NULL;
