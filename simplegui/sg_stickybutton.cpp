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

#include <GL/gl.h>

#include "sg_stickybutton.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_StickyButton::SG_StickyButton(string mes, int c, int cc, int pc)
	: SG_Button(mes, c, cc) {
  texture.push_back(pc);
  on = 0;
  }

SG_StickyButton::~SG_StickyButton() {
  }

bool SG_StickyButton::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "StickyButton/Handle: Button Down at (%f,%f)\n", x, y);

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    flags |= SG_WIDGET_FLAGS_PRESSED;
    state = 1;
    on = !on;
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_STICKYOFF + on;
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
    state = 3 * on;
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_BUTTONRELEASE;
    event->user.data1 = (void*)this;
    event->user.data2 = NULL;
    return 1;
    }  

  return 1;
  }

//  bool SG_StickyButton::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_StickyButton::Default_Mouse_Cursor = NULL;

