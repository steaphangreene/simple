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

#include "SDL_opengl.h"

#include "sg_stickybutton.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_StickyButton::SG_StickyButton(string mes, SG_Texture tex,
	SG_Texture dis_tex, SG_Texture click_tex, SG_Texture down_tex)
	: SG_Button(mes, tex, dis_tex, click_tex) {
  texture.push_back(down_tex);
  }

SG_StickyButton::~SG_StickyButton() {
  }

int SG_StickyButton::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "StickyButton/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    flags |= SG_WIDGET_FLAGS_PRESSED;
    flags ^= SG_WIDGET_FLAGS_ON;
    state = 2;
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_STICKYOFF + ((flags & SG_WIDGET_FLAGS_ON) > 0);
    event->user.data1 = (void*)this;
    event->user.data2 = NULL;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    flags &= (~SG_WIDGET_FLAGS_PRESSED);
    state = 3 * ((flags & SG_WIDGET_FLAGS_ON) > 0);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONRELEASE;
    event->user.data1 = (void*)this;
    event->user.data2 = NULL;
    return 1;
    }  

  return 1;
  }

void SG_StickyButton::TurnOn() {
  flags |= SG_WIDGET_FLAGS_ON;
  if(state == 0) state = 3;
  }

void SG_StickyButton::TurnOff() {
  flags &= (~SG_WIDGET_FLAGS_ON);
  if(state == 3) state = 0;
  }

//  bool SG_StickyButton::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_StickyButton::Default_Mouse_Cursor = NULL;

