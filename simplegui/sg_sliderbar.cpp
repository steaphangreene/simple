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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#include "SDL_opengl.h"

#include "sg_sliderbar.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_dragable.h"
#include "sg_events.h"

SG_SliderBar::SG_SliderBar(bool vert,
        SimpleTexture b1tex, SimpleTexture b1tex_dis, SimpleTexture b1tex_click,
        SimpleTexture b2tex, SimpleTexture b2tex_dis, SimpleTexture b2tex_click,
        SimpleTexture handtex, SimpleTexture bgtex)
	: SG_Compound(1, 1, 0.0, 0.0), SG_Ranger() {
  vertical = vert;
  SetBackground(new SG_Panel(bgtex));
  handle = new SG_Dragable(handtex);

  if(vertical) {
    Resize(1, 11);
    incb = new SG_Button("^", b1tex, b1tex_dis, b1tex_click);
    decb = new SG_Button("v", b2tex, b2tex_dis, b2tex_click);
    AddWidget(incb, 0, 0);
    AddWidget(decb, 0, ysize-1);
    AddWidget(handle, 0, 1);	//Hardcoded initial placement (for now)
    handle->SetYDisplayLimits(0.0, 16.0);
    handle->LinkYFrom(this);
    }
  else {
    Resize(11, 1);
    incb = new SG_Button(">", b1tex, b1tex_dis, b1tex_click);
    decb = new SG_Button("<", b2tex, b2tex_dis, b2tex_click);
    AddWidget(incb, xsize-1, 0);
    AddWidget(decb, 0, 0);
    AddWidget(handle, 1, 0);	//Hardcoded initial placement (for now)
    handle->SetXDisplayLimits(0.0, 16.0);
    handle->LinkXFrom(this);
    }
  }

SG_SliderBar::~SG_SliderBar() {
  }

static SG_Event_DataType event_data;

int SG_SliderBar::HandleEvent(SDL_Event *event, float x, float y) {
  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 4) {
    SetValue(Value() - 0.5);	//FIXME: Use Increment Size
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 5) {
    SetValue(Value() + 0.5);	//FIXME: Use Increment Size
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  return SG_Compound::HandleEvent(event, x, y);
  }

bool SG_SliderBar::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    // FIXME: Unimplemented
    }
  else if(event->user.code == SG_EVENT_MOVE2D) {
    event->user.code = SG_EVENT_MOVE;
    event->user.data1 = (void*)(SG_Ranger*)this;
    event_data.f[0] = Value();
    event->user.data2 = (void*)&event_data;
    return 1;
    }
  return 0; // Silence children doing other things
  }

//  bool SG_SliderBar::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_SliderBar::Default_Mouse_Cursor = NULL;

bool SG_SliderBar::Render(unsigned long cur_time) {
  return SG_Compound::Render(cur_time);
  }
