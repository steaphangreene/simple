// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#include "SDL_opengl.h"

#include "sg_multitab.h"
#include "sg_tabs.h"
#include "sg_events.h"

SG_MultiTab::SG_MultiTab(const vector<string> &items,
	const vector<SG_Alignment *> &areas,
	int tinvpro,
	SimpleTexture ttex, SimpleTexture dis_ttex, SimpleTexture click_ttex,
	SimpleTexture down_ttex) : SG_Compound(1, tinvpro, 0.0, 0.0) {

  tabs = new SG_Tabs(items, SG_AUTOSIZE, 1);
  AddWidget(tabs, 0, 0, 1, 1);

  subscreens = areas;
  if(subscreens.size() < items.size()) subscreens.resize(items.size());
  for(unsigned int anum = 0; anum != subscreens.size(); ++anum) {
    if(!subscreens[anum]) subscreens[anum] = new SG_Alignment();
    }

  if(items.size() > 0) AddWidget(subscreens[tabs->Which()], 0, 1, 1, ysize - 1);
  }

SG_MultiTab::~SG_MultiTab() {
  if(widgets.size() > 1) RemoveWidget(widgets[1]);
  vector<SG_Alignment *>::iterator itrw = subscreens.begin();
  for(; itrw != subscreens.end(); ++itrw) {
    if(*itrw) delete (*itrw);
    }
  subscreens.clear();
  }

const string &SG_MultiTab::Item(int opt) {
  return tabs->Item(opt);
  }

void SG_MultiTab::SetItems(const vector<string> &items) {
  tabs->SetItems(items);
  if(widgets.size() > 1) RemoveWidget(widgets[1]);
  while(subscreens.size() > 0) delete subscreens[0];
  subscreens.clear();
  if(subscreens.size() < items.size()) subscreens.resize(items.size());
  Set(tabs->Which());
  }

void SG_MultiTab::SetArea(int page, SG_Alignment *area) {
  if(page < 0 || page >= int(subscreens.size())) return;
  if(subscreens[page]) {
    if(page == tabs->Which()) RemoveWidget(subscreens[page]);
    delete subscreens[page];
    }
  subscreens[page] = area;
  }

void SG_MultiTab::SetAreas(const vector<SG_Alignment *> &areas) {
  for(Uint32 i = 0; i < subscreens.size(); ++i) {
    delete subscreens[i];
    }
  subscreens = areas;
  if(int(subscreens.size()) < tabs->NumItems())
	subscreens.resize(tabs->NumItems());
  Set(tabs->Which());
  }

int SG_MultiTab::NumItems() {
  return tabs->NumItems();
  }

void SG_MultiTab::Set(int which) {
  if(which >= int(subscreens.size()) || which < 0) return;
  if(tabs->Which() != which) tabs->Set(which);
  if(widgets.size() > 1)
    RemoveWidget(widgets[1]); //Always REALLY current subwidget
  if(which < int(subscreens.size()) && subscreens[which])
    AddWidget(subscreens[which], 0, 1, 1, ysize - 1);
  }

bool SG_MultiTab::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_SELECT) {
    Set(*((int *)(event->user.data2)));
    event->user.data1 = (void*)(SG_Compound*)this;  //Now it's MY select event
    return 1;
    }
  return 1; // Don't silence children doing other things
  }

//  bool SG_MultiTab::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_MultiTab::Default_Mouse_Cursor = NULL;
