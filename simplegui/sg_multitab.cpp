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

#include "sg_multitab.h"
#include "sg_tabs.h"
#include "sg_events.h"

SG_MultiTab::SG_MultiTab(vector<string> items, vector<SG_Alignment *> areas,
	int tinvpro,
	SG_Texture ttex, SG_Texture dis_ttex, SG_Texture click_ttex,
	SG_Texture down_ttex) : SG_Compound(1, tinvpro, 0.0, 0.0) {
  if(items.size() != areas.size()) {
    fprintf(stderr, "ERROR: SG_MultiTab given vectors of different sizes!\n");
    exit(1);
    }
  if(items.size() == 0) {
    fprintf(stderr, "ERROR: SG_MultiTab given vectors of zero size!\n");
    exit(1);
    }
  SG_Tabs *wid =
    new SG_Tabs(items, SG_AUTOSIZE, 1, ttex, dis_ttex, click_ttex, down_ttex);
  AddWidget(wid, 0, 0, 1, 1);
  AddWidget(areas[((SG_Tabs *)(widgets[0]))->Which()], 0, 1, 1, ysize - 1);
  subscreens = areas;
  }

SG_MultiTab::~SG_MultiTab() {
  RemoveWidget(widgets[1]);
  vector<SG_Alignment *> tmp = subscreens;
  vector<SG_Alignment *>::iterator itrw = tmp.begin();
  subscreens.clear();
  for(; itrw != tmp.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  }

void SG_MultiTab::Set(int which) {
  RemoveWidget(widgets[1]); //Always REALLY current subwidget
  AddWidget(subscreens[which], 0, 1, 1, ysize - 1);
  }

bool SG_MultiTab::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_SELECT) {
    Set(*((int *)(event->user.data2)));
    event->user.data1 = (void*) this;	//Now it's MY select event
    return 1;
    }
  return 0; // Silence children doing other things
  }

//  bool SG_MultiTab::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_MultiTab::Default_Mouse_Cursor = NULL;
