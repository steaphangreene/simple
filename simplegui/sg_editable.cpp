// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#include "sg_editable.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Editable::SG_Editable(string mes, SimpleTexture c, 
		SimpleTexture dc, SimpleTexture fc)
	: SG_TextArea(mes, c) {
  dc.LinkTextFrom(&texture[0]);
  texture.push_back(dc);
  fc.LinkTextFrom(&texture[0]);
  texture.push_back(fc);
  }

SG_Editable::~SG_Editable() {
  }

int SG_Editable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Editable/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    current_sg->SetCurrentWidget(this);
    current_sg->SetFocusWidget(this);
    state = 2;
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) { // Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    current_sg->UnsetCurrentWidget();
    return 0;
    }
  else if(event->type == SDL_KEYDOWN) {
    if(event->key.keysym.sym == SDLK_ESCAPE) {	// Revert?
      current_sg->UnsetFocusWidget();
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_EDITABORT;
      event->user.data1 = (void*)(SG_Text*)this;
      event->user.data2 = NULL;
      return 1;
      }
    else if(event->key.keysym.sym == SDLK_BACKSPACE) {
      if(message.length() > 0) {
	string newmes = message.substr(0, message.length() - 1);
	SetText(newmes);
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_EDIT;
	event->user.data1 = (void*)(SG_Text*)this;
	event->user.data2 = NULL;
	return 1;
	}
      return 0;
      }
    else if(event->key.keysym.sym == SDLK_RETURN
	|| event->key.keysym.sym == SDLK_KP_ENTER) {
      if(texture[0].GetTextGeometry()->visible_ylines == 1.0) {
	current_sg->UnsetFocusWidget();
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_NEWTEXT;
	event->user.data1 = (void*)(SG_Text*)this;
	event->user.data2 = NULL;
	return 1;
	}
      else {
	string newmes = message.append(1, '\n');
	SetText(newmes);
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_EDIT;
	event->user.data1 = (void*)(SG_Text*)this;
	event->user.data2 = NULL;
	return 1;
	}
      }
    else if(event->key.keysym.unicode < 0x80) {
      if(isgraph(event->key.keysym.unicode)) {
	string newmes = message.append(1, (char)(event->key.keysym.unicode));
	SetText(newmes);
	event->type = SDL_SG_EVENT;
	event->user.code = SG_EVENT_EDIT;
	event->user.data1 = (void*)(SG_Text*)this;
	event->user.data2 = NULL;
	return 1;
	}
      }
    return 0;  //Eat keyboard events I don't handle.
    }
  else if(event->type == SDL_KEYUP) {
    return 0;  //Eat keyboard events I don't handle.
    }

  return 1;
  }

bool SG_Editable::RenderSelf(unsigned long cur_time) {
  if(state == 2 && current_sg->FocusWidget() != this) state = 0;
  return SG_Panel::RenderSelf(cur_time);
  }

//  bool SG_Editable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Editable::Default_Mouse_Cursor = NULL;
