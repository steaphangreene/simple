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

#include "sg_events.h"
#include "sg_compound.h"

SG_Compound::SG_Compound(int xsz, int ysz, float xbor, float ybor)
	: SG_Table(xsz, ysz, xbor, ybor) {
  }

SG_Compound::~SG_Compound() {
  }

int SG_Compound::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Compound/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  int ret = SG_Table::HandleEvent(event, x, y);
  if(ret > 0 && event->type == SDL_SG_EVENT) {
    ret = ChildEvent(event);
    if(!ret) {
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_NEEDTORENDER;
      ret = 1;
      }
    }
  return ret;
  }

bool SG_Compound::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONUP)
//    fprintf(stderr, "Compound/Hand: Button Up at (%f,%f)\n", x, y);

  if(targ == this) return HandleEvent(event, x, y);

  int ret = SG_Table::HandEventTo(targ, event, x, y);
  if(ret && event->type == SDL_SG_EVENT) {
    ret = ChildEvent(event);
    if(!ret) {
      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_NEEDTORENDER;
      ret = 1;
      }
    }
  return ret;
  }

bool SG_Compound::ChildEvent(SDL_Event *event) {
  return 1;
  }

//  bool SG_Compound::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Compound::Default_Mouse_Cursor = NULL;
