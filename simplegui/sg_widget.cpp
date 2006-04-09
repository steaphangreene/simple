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

#include "sg_alignment.h"
#include "sg_globals.h"

SG_Widget::SG_Widget() {
  flags = 0;
  }

SG_Widget::~SG_Widget() {
  if(current_sg->CurrentWidget() == this) current_sg->UnsetCurrentWidget();
  if(current_sg->FocusWidget() == this) current_sg->UnsetFocusWidget();
  if(current_sg->PopupWidget() == this) current_sg->UnsetPopupWidget();
  }

int SG_Widget::HandleEvent(SDL_Event *event, float x, float y) {
  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events
  if(flags & SG_WIDGET_FLAGS_DISABLED) return 0; //Eat all events

  if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }

  return 1;
  }

bool SG_Widget::HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y) {
  if(targ == this) return HandleEvent(event, x, y);
  return 1;
  }

bool SG_Widget::HasWidget(SG_Widget *targ) {
  if(flags & SG_WIDGET_FLAGS_HIDDEN) return true;

  return this == targ;
  }

bool SG_Widget::Render(unsigned long cur_time) {
  return RenderSelf(cur_time);
  }

bool SG_Widget::RenderSelf(unsigned long cur_time) {
  return 1;
  }

void SG_Widget::Disable() {
  flags |= SG_WIDGET_FLAGS_DISABLED;
  }

void SG_Widget::Enable() {
  flags &= (~SG_WIDGET_FLAGS_DISABLED);
  }

void SG_Widget::TurnOn() {
  flags |= SG_WIDGET_FLAGS_ON;
  }

void SG_Widget::TurnOff() {
  flags &= (~SG_WIDGET_FLAGS_ON);
  }

//  bool SG_Widget::SetDefaultCursor(GL_MODEL *cur);
//  bool SG_Widget::SetCursor(GL_MODEL *cur)
//  bool SG_Widget::UnsetCursor()
  
//  static GL_MODEL SG_Widget::Default_Mouse_Cursor = NULL;

void SG_Widget::AdjustGeometry(SG_AlignmentGeometry *geom) {
  }

void SG_Widget::SetAspectRatio(double asp) {
  aspect_ratio = asp;
  }
