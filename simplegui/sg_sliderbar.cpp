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

  if(!stt_slider) stt_slider = new STT_ButtonUp;

  if(vertical) {
    Resize(1, 11);
    incb = new SG_Button("", b1tex, b1tex_dis, b1tex_click);
    decb = new SG_Button("", b2tex, b2tex_dis, b2tex_click);
    incb->SetTexturator(stt_dnbutt_up, 0);
    incb->SetTexturator(stt_dnbutt_dn, 2);
    decb->SetTexturator(stt_upbutt_up, 0);
    decb->SetTexturator(stt_upbutt_dn, 2);
    AddWidget(incb, 0, ysize-1);
    AddWidget(decb, 0, 0);
    AddWidget(handle, 0, 1);	//Hardcoded initial placement (for now)
    handle->SetYDisplayLimits(0.0, 16.0);
    handle->LinkYFrom(this);
    handle->SetTexturator(stt_slider, 0);
    }
  else {
    Resize(11, 1);
    incb = new SG_Button("", b1tex, b1tex_dis, b1tex_click);
    decb = new SG_Button("", b2tex, b2tex_dis, b2tex_click);
    incb->SetTexturator(stt_rtbutt_up, 0);
    incb->SetTexturator(stt_rtbutt_dn, 2);
    decb->SetTexturator(stt_ltbutt_up, 0);
    decb->SetTexturator(stt_ltbutt_dn, 2);
    AddWidget(incb, xsize-1, 0);
    AddWidget(decb, 0, 0);
    AddWidget(handle, 1, 0);	//Hardcoded initial placement (for now)
    handle->SetXDisplayLimits(0.0, 16.0);
    handle->LinkXFrom(this);
    handle->SetTexturator(stt_slider, 0);
    }
  }

SG_SliderBar::~SG_SliderBar() {
  }

static SG_Event_DataType event_data;

int SG_SliderBar::HandleEvent(SDL_Event *event, float x, float y) {
  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEWHEEL && event->wheel.y < 1) {
    Decrement();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_NEEDTORENDER;
    event->user.data1 = NULL;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->type == SDL_MOUSEWHEEL && event->wheel.y > 1) {
    Increment();
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
    if((SG_Widget*)(event->user.data1) == (SG_Widget*)(incb)) {
      Increment();
      }
    else {
      Decrement();
      }
    event->user.code = SG_EVENT_MOVE;
    event->user.data1 = (void*)(SG_Ranger*)this;
    event_data.f[0] = Value();
    event->user.data2 = (void*)&event_data;
    return 1;
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

bool SG_SliderBar::RenderSelf(unsigned long cur_time) {
  return SG_Compound::RenderSelf(cur_time);
  }

ST_Texturator *SG_SliderBar::stt_slider = NULL;
