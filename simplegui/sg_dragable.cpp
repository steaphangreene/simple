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

#include "sg_dragable.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Dragable::SG_Dragable(SG_Texture tex) : SG_Panel(tex) {
  max_x = 0.0;
  min_x = 0.0;
  max_y = 0.0;
  min_y = 0.0;
  }

SG_Dragable::~SG_Dragable() {
  }

void SG_Dragable::SetLimits(float mnx, float mny, float mxx, float mxy) {
  if(mxx < 0.0 || mxy < 0.0 || mnx > 0.0 || mny > 0.0) {
    fprintf(stderr, "WARNING: Invalid Limits to Dragable!\n");
    return;
    }
  min_x = mnx;
  min_y = mny;
  max_x = mxx;
  max_y = mxy;
  }

int SG_Dragable::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Dragable/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  static float event_data[2];

  if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == 1) {
    current_sg->SetCurrentWidget(this);
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_DRAGGRAB;
    event->user.data1 = (void*)this;
    event->user.data2 = NULL;
    base_x = x;
    base_y = y;
    off_x = 0;
    off_y = 0;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEMOTION) {
    if(current_sg->CurrentWidget() == this) {
      off_x = x - base_x;
      if(off_x > max_x) off_x = max_x;
      if(off_x < min_x) off_x = min_x;
      off_y = y - base_y;
      if(off_y > max_y) off_y = max_y;
      if(off_y < min_y) off_y = min_y;
      event_data[0] = off_x;
      event_data[1] = off_x;

      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_DRAGMOVE;
      event->user.data1 = (void*)this;
      event->user.data2 = (void*)event_data;
      return 1;
      }
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    off_x = x - base_x;
    if(off_x > max_x) off_x = max_x;
    if(off_x < min_x) off_x = min_x;
    off_y = y - base_y;
    if(off_y > max_y) off_y = max_y;
    if(off_y < min_y) off_y = min_y;
    event_data[0] = off_x;
    event_data[1] = off_x;

    current_sg->UnsetCurrentWidget();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_DRAGRELEASE;
    event->user.data1 = (void*)this;
    event->user.data2 = (void*)event_data;
    return 1;
    }  

  return 1;
  }

//  bool SG_Dragable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Dragable::Default_Mouse_Cursor = NULL;

bool SG_Dragable::Render(unsigned long cur_time) {
  if(current_sg->CurrentWidget() == this) {
    glTranslatef(off_x, off_y, 0.0);
    }
  return SG_Panel::Render(cur_time);
  }
