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

#include "sg_editable.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Editable::SG_Editable(string mes, int c, int dc, int fc)
		: SG_TextArea(mes, c) {
  texture.push_back(dc);
  texture.push_back(fc);
  }

SG_Editable::~SG_Editable() {
  }

bool SG_Editable::HandleKeyboardEvent(SDL_Event *event) {
  if(event->type == SDL_KEYDOWN) {
    if(event->key.keysym.sym == SDLK_ESCAPE) {	// Revert?
      current_sg->UnsetFocusWidget();
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_EDITABORT;
      event->user.data1 = (void*)this;
      event->user.data2 = NULL;
      return 1;
      }
    else if(event->key.keysym.sym == SDLK_BACKSPACE) {
      if(message.length() > 0) {
	string newmes = message.substr(0, message.length() - 1);
	SetText(newmes);
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_EDIT;
	event->user.data1 = (void*)this;
	event->user.data2 = NULL;
	return 1;
	}
      return 0;
      }
    else if(event->key.keysym.sym == SDLK_RETURN
	|| event->key.keysym.sym == SDLK_KP_ENTER) {
      current_sg->UnsetFocusWidget();
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_EDITCOMPLETE;
      event->user.data1 = (void*)this;
      event->user.data2 = NULL;
      return 1;
      }
    else if(event->key.keysym.unicode < 0x80) {
      if(isgraph(event->key.keysym.unicode)) {
	string newmes = message.append(1, (char)(event->key.keysym.unicode));
	SetText(newmes);
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_EDIT;
	event->user.data1 = (void*)this;
	event->user.data2 = NULL;
	return 1;
	}
      }
    }
  return 0;  //Eat keyboard events I don't handle.
  }

bool SG_Editable::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Editable/Handle: Button Down at (%f,%f)\n", x, y);

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    current_sg->SetCurrentWidget(this);
    current_sg->SetFocusWidget(this);
    state = 2;
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) { // Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    return 0;
    }

  return 1;
  }

bool SG_Editable::Render(unsigned long cur_time) {
  if(state == 2 && current_sg->FocusWidget() != this) state = 0;
  return SG_Panel::Render(cur_time);
  }

//  bool SG_Editable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Editable::Default_Mouse_Cursor = NULL;
