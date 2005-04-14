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

#include "sg_dragable.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Dragable::SG_Dragable(SG_Texture tex) : SG_Panel(tex),
	SG_Ranger2D(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) {
  }

SG_Dragable::~SG_Dragable() {
  }

int SG_Dragable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Dragable/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  static SG_Event_DataType event_data;

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_DRAGGRAB;
    event->user.data1 = (void*)this;
    event->user.data2 = NULL;
    base_x = x;
    base_y = y;
    start_x = XOffset();
    start_y = YOffset();
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEMOTION) {
    if(current_sg->CurrentWidget() == this) {
      SetXOffset(x - base_x);
      SetYOffset(y - base_y);
      event_data.f[0] = XOffset();
      event_data.f[1] = YOffset();

      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_DRAGMOVE;
      event->user.data1 = (void*)this;
      event->user.data2 = (void*)&event_data;
      return 1;
      }
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    SetXOffset(x - base_x);
    SetYOffset(y - base_y);
    event_data.f[0] = XOffset();
    event_data.f[1] = YOffset();

    current_sg->UnsetCurrentWidget();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_DRAGRELEASE;
    event->user.data1 = (void*)this;
    event->user.data2 = (void*)&event_data;
    return 1;
    }  

  return 1;
  }

//  bool SG_Dragable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Dragable::Default_Mouse_Cursor = NULL;

bool SG_Dragable::Render(unsigned long cur_time) {
  if(current_sg->CurrentWidget() == this) {
    glTranslatef(XOffset(), YOffset(), 0.0);
    }
  return SG_Panel::Render(cur_time);
  }
