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

#include "sg_tabs.h"
#include "sg_stickybutton.h"
#include "sg_events.h"

SG_Tabs::SG_Tabs(vector<string> items, int x, int y, SimpleTexture tex,
	SimpleTexture dis_tex, SimpleTexture click_tex, SimpleTexture down_tex)
	: SG_Compound(
		(x<=0) ? items.size(): x,
		(y<=0) ? items.size(): y,
		0.0, 0.0) {
  cur_on = 0;
  for(int n=0; n < (int)(items.size()); ++n) {
    SG_StickyButton * sb =
	new SG_StickyButton(items[n], tex, dis_tex, click_tex, down_tex);
    AddWidget(sb);
    if(n == cur_on) sb->TurnOn();
    }
  }

SG_Tabs::~SG_Tabs() {
  }

bool SG_Tabs::ChildEvent(SDL_Event *event) {
  static SG_Event_DataType event_data;

  if(event->user.code == SG_EVENT_STICKYON) {
    vector<SG_Widget *>::iterator itrw = widgets.begin();
    for(; itrw != widgets.end(); ++itrw) {
      if((*itrw) && (*itrw) != event->user.data1) (*itrw)->TurnOff();
      else if(*itrw) cur_on = itrw - widgets.begin();
      }
    event_data.i[0] = cur_on;
    event->user.code = SG_EVENT_SELECT;
    event->user.data1 = (void*)(SG_Compound*)this;
    event->user.data2 = (void*)&event_data;
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
  SetActive(wgeom[which].xpos, wgeom[which].ypos,
	wgeom[which].xpos + wgeom[which].xsize,
	wgeom[which].ypos + wgeom[which].ysize
	);
  cur_on = which;
  }

void SG_Tabs::Next(bool wrap) {
  ++cur_on;
  if(cur_on >= xsize*ysize) {
    if(wrap) cur_on = 0;
    else cur_on = xsize*ysize-1;
    }
  Set(cur_on);
  }

void SG_Tabs::Prev(bool wrap) {
  --cur_on;
  if(cur_on < 0) {
    if(wrap) cur_on = xsize*ysize-1;
    else cur_on = 0;
    }
  Set(cur_on);
  }

void SG_Tabs::Right(bool wrap) {
  ++cur_on;
  if(cur_on % xsize == 0) {
    if(wrap) cur_on -= xsize;
    else --cur_on;
    }
  Set(cur_on);
  }

void SG_Tabs::Left(bool wrap) {
  --cur_on;
  if(cur_on < 0 || cur_on % xsize == (xsize-1)) {
    if(wrap) cur_on += xsize;
    else ++cur_on;
    }
  Set(cur_on);
  }

void SG_Tabs::Up(bool wrap) {
  cur_on -= xsize;
  if(cur_on < 0) {
    if(wrap) cur_on += xsize*ysize;
    else cur_on += xsize;
    }
  Set(cur_on);
  }

void SG_Tabs::Down(bool wrap) {
  cur_on += xsize;
  if(cur_on >= xsize*ysize) {
    if(wrap) cur_on -= xsize*ysize;
    else cur_on -= xsize;
    }
  Set(cur_on);
  }
