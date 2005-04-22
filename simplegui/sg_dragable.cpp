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

#include <cmath>
using namespace std;

#include "SDL_opengl.h"

#include "sg_dragable.h"
#include "sg_alignment.h"
#include "sg_events.h"
#include "sg_globals.h"

SG_Dragable::SG_Dragable(SG_Texture tex) : SG_Panel(tex),
	SG_Ranger2D(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) {
  SetDisplayLimits(0.0, 0.0, 0.0, 0.0);
  start_x = 0.0;
  start_y = 0.0;
  base_x = 0.0;
  base_y = 0.0;
  off_x = 0.0;
  off_y = 0.0;
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
    event->user.data1 = (void*)(SG_Ranger2D*)this;
    event->user.data2 = NULL;
    start_x = XValue();
    start_y = YValue();
    base_x = start_x;
    base_y = start_y;
    Limits2Disp(base_x, base_y);
    off_x = x - base_x;
    off_y = y - base_y;
    return 1;
    }
  else if(event->type == SDL_MOUSEBUTTONDOWN) {	// Eat other buttons
    return 0;
    }
  else if(event->type == SDL_MOUSEMOTION) {
    if(current_sg->CurrentWidget() == this) {
      x -= off_x;  y -= off_y;
      Disp2Limits(x, y);
      SetXValue(x);
      SetYValue(y);
      event_data.f[0] = XValue();
      event_data.f[1] = YValue();

      event->type = SDL_SG_EVENT;
      event->user.code = SG_EVENT_DRAGMOVE;
      event->user.data1 = (void*)(SG_Ranger2D*)this;
      event->user.data2 = (void*)&event_data;
      return 1;
      }
    return 0;
    }
  else if(event->type == SDL_MOUSEBUTTONUP) {
    x -= off_x;  y -= off_y;
    Disp2Limits(x, y);
    SetXValue(x);
    SetYValue(y);
    event_data.f[0] = XValue();
    event_data.f[1] = YValue();

    start_x = 0.0;
    start_y = 0.0;
    base_x = 0.0;
    base_y = 0.0;

    current_sg->UnsetCurrentWidget();
    event->type = SDL_SG_EVENT;
    event->user.code = SG_EVENT_DRAGRELEASE;
    event->user.data1 = (void*)(SG_Ranger2D*)this;
    event->user.data2 = (void*)&event_data;
    return 1;
    }  

  return 1;
  }

//  bool SG_Dragable::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Dragable::Default_Mouse_Cursor = NULL;

bool SG_Dragable::Render(unsigned long cur_time) {
//  if(current_sg->CurrentWidget() == this) {
//    float xprog = XValue(), yprog = YValue();
//    Limits2Disp(xprog, yprog);
//    glTranslatef(xprog, yprog, 0.0);
//    }
  return SG_Panel::Render(cur_time);
  }

void SG_Dragable::SetDisplayLimits(float mnx, float mny, float mxx, float mxy) {
  min_dx = mnx;
  min_dy = mny;
  max_dx = mxx;
  max_dy = mxy;
  }

void SG_Dragable::SetXDisplayLimits(float mnx, float mxx) {
  min_dx = mnx;
  max_dx = mxx;
  }

void SG_Dragable::SetYDisplayLimits(float mny, float mxy) {
  min_dy = mny;
  max_dy = mxy;
  }

void SG_Dragable::Disp2Limits(float &x, float &y) {
  if(min_dx != max_dx && fabs(XMax() - XMin()) > XSpan()) {
    x -= min_dx + base_x;
    x /= fabs(max_dx - min_dx);
    x *= (fabs(XMax() - XMin()) - XSpan());
    x += XMin() + start_x;
    }
  else x = XMin();

  if(min_dy != max_dy && fabs(YMax() - YMin()) > YSpan()) {
    y -= -min_dy + base_y;
    y /= fabs(max_dy - min_dy);
    y = -y;
    y *= (fabs(YMax() - YMin()) - YSpan());
    y += YMin() + start_y;
    }
  else y = YMin();
  }

void SG_Dragable::Limits2Disp(float &x, float &y) {
  if(min_dx != max_dx && fabs(XMax() - XMin()) > XSpan()) {
    x -= XMin();
    x /= (fabs(XMax() - XMin()) - XSpan());
    x *= fabs(max_dx - min_dx);
    x += min_dx;
    }
  else x = min_dx;

  if(min_dy != max_dy && fabs(YMax() - YMin()) > YSpan()) {
    y -= YMin();
    y /= (fabs(YMax() - YMin()) - YSpan());
    y = -y;
    y *= fabs(max_dy - min_dy);
    y += -min_dy;
    }
  else y = min_dy;
  }

void SG_Dragable::AdjustGeometry(SG_AlignmentGeometry *geom) {
  float xprog = XValue(), yprog = YValue();
  Limits2Disp(xprog, yprog);
  geom->xp += xprog * geom->xs;
  geom->yp += yprog * geom->ys;
  }
