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

#include "sg_scrollingarea.h"
#include "sg_textarea.h"
#include "sg_events.h"

SG_ScrollingArea::SG_ScrollingArea(float xsz, float ysz, float xvs, float yvs)
	: SG_Compound(41, 41, 0.0, 0.0) {
  scroll = new SG_Scrollable((xvs < 1.0) ? xsz : xvs, (yvs < 1.0) ? ysz : yvs,
	0.0, 0.0, 0.0, 0.0, xsz, ysz);
  LinkFrom(scroll);
  SG_Compound::AddWidget(scroll, 0, 0, 40, 40);

  horiz = new SG_SliderBar(false);
  scroll->LinkXTo(horiz);
  SG_Compound::AddWidget(horiz, 0, 40, 40, 1);

  vert = new SG_SliderBar(true);
  scroll->LinkYTo(vert);
  SG_Compound::AddWidget(vert, 40, 0, 1, 40);
  }

SG_ScrollingArea::~SG_ScrollingArea() {
  }

bool SG_ScrollingArea::ChildEvent(SDL_Event *event) {
  if(event->user.data1 == (void*)(SG_Ranger*)horiz) {
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1; // Silence my children doing things
    }
  else if(event->user.data1 == (void*)(SG_Ranger*)vert) {
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1; // Silence my children doing things
    }
  return 1; // Others can do whatever they want
  }

//  bool SG_ScrollingArea::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_ScrollingArea::Default_Mouse_Cursor = NULL;

bool SG_ScrollingArea::AddWidget(SG_Widget *wid) {
  return scroll->AddWidget(wid);
  }

void SG_ScrollingArea::RemoveWidget(SG_Widget *wid) {
  scroll->RemoveWidget(wid);
  }

bool SG_ScrollingArea::AddWidget(SG_Table *tab) {
  return scroll->AddWidget(tab);
  }

bool SG_ScrollingArea::AddWidget(SG_TextArea *text) {
  return scroll->AddWidget(text);
  }
