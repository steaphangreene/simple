// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#include "SDL_opengl.h"

#include "sg_button.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Button::SG_Button(const string &mes, SimpleTexture tex, SimpleTexture dis_tex,
	SimpleTexture click_tex) : SG_TextArea(mes, tex, dis_tex) {
  click_tex.LinkTextFrom(&(texture[0]));
  texture.push_back(click_tex);

  if(!stt_butt_up) stt_butt_up = new STT_ButtonUp;
  if(!stt_butt_dn) stt_butt_dn = new STT_ButtonDown;

  texture[0].SetTexturator(stt_butt_up);
  texture[2].SetTexturator(stt_butt_dn);
  }

SG_Button::~SG_Button() {
  }

int SG_Button::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Button/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    flags |= SG_WIDGET_FLAGS_PRESSED;
    state = 2;
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONPRESS;
    event->user.data1 = (void*)(SG_Widget*)this;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    flags &= (~SG_WIDGET_FLAGS_PRESSED);
    state = 0;
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONRELEASE;
    event->user.data1 = (void*)(SG_Widget*)this;
    event->user.data2 = NULL;
    if(x >= -1.0 && y >= -1.0 && x <= 1.0 && y <= 1.0)
      event->user.code = SG_EVENT_BUTTONCLICK;
    return 1;
    }

  return 1;
  }

//  bool SG_Button::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_Button::Default_Mouse_Cursor = NULL;

ST_Texturator *SG_Button::stt_butt_up = NULL;
ST_Texturator *SG_Button::stt_butt_dn = NULL;
