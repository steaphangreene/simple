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

#include "sg_tabs.h"
#include "sg_stickybutton.h"
#include "sg_events.h"

SG_Tabs::SG_Tabs(vector<string> items, int x, int y, SG_Texture tex,
	SG_Texture dis_tex, SG_Texture click_tex, SG_Texture down_tex)
	: SG_Compound(
		(x<0) ? items.size(): 1,
		(y<0) ? items.size(): 1,
		0.0, 0.0) {
  for(int n=0; n < (int)(items.size()); ++n) {
    SG_StickyButton * sb =
	new SG_StickyButton(items[n], tex, dis_tex, click_tex, down_tex);
    AddWidget(sb);
    if(n == 0) sb->TurnOn();
    }
  }

SG_Tabs::~SG_Tabs() {
  }

bool SG_Tabs::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_STICKYON) {
    vector<SG_Widget *>::iterator itrw = widgets.begin();
    for(; itrw != widgets.end(); ++itrw) {
      if((*itrw) && (*itrw) != event->user.data1) (*itrw)->TurnOff();
      else if(*itrw) cur_on = itrw - widgets.begin();
      }
    event->user.code = SG_EVENT_SELECT;
    event->user.data1 = (void*)this;
    event->user.data2 = (void*)cur_on;
    return 1;
    }
  if(event->user.code == SG_EVENT_STICKYOFF) {
    ((SG_Widget *)(event->user.data1))->TurnOn();
    return 0;
    }
  return 0; // Silence children doing other things
  }

//  bool SG_Tabs::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Tabs::Default_Mouse_Cursor = NULL;

void SG_Tabs::Set(int which) {
  if(which < 0 || which >= (int)(widgets.size())) return; //Ignore invalid

  vector<SG_Widget *>::iterator itrw = widgets.begin();
  for(; itrw != widgets.end(); ++itrw) {
    if(*itrw) (*itrw)->TurnOff();
    }
  widgets[which]->TurnOn();
  cur_on = which;
  }

