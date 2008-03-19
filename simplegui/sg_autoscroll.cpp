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

#include "sg_autoscroll.h"

SG_AutoScroll::SG_AutoScroll(float xspn, float yspn, float xval, float yval,
	float xmin, float ymin, float xmax, float ymax)
	: SG_Scrollable(xspn, yspn, xval, yval, xmin, ymin, xmax, ymax) {
  xstart = 0.0;
  ystart = 0.0;
  xend = 0.0;
  yend = 0.0;
  xduration = 0.0;
  yduration = 0.0;
  xstart_time = 0;
  ystart_time = 0;
  }

SG_AutoScroll::~SG_AutoScroll() {
  }

bool SG_AutoScroll::RenderSelf(unsigned long cur_time) {
  if(xstart != xend && xduration > 0.0) {
    double part = (double)(SDL_GetTicks() - xstart_time) / (xduration * 1000.0);
    while(part >= 1.0) part -= 1.0;
    SetXValue(xstart * (1.0 - part) + xend * part);
    }
  if(ystart != yend && yduration > 0.0) {
    double part = (double)(SDL_GetTicks() - ystart_time) / (yduration * 1000.0);
    while(part >= 1.0) part -= 1.0;
    SetYValue(ystart * (1.0 - part) + yend * part);
    }
  return SG_Scrollable::RenderSelf(cur_time);
  }

void SG_AutoScroll::SetXScroll(float start, float end, float secs, Uint32 cur_time) {
  xstart = start;
  xend = end;
  xduration = secs;
  xstart_time = cur_time;
  if(xstart_time == 0) xstart_time = SDL_GetTicks();
  }

void SG_AutoScroll::StopXScroll() {
  xstart = 0.0;
  xend = 0.0;
  xstart_time = 0;
  }

void SG_AutoScroll::SetYScroll(float start, float end, float secs, Uint32 cur_time) {
  ystart = start;
  yend = end;
  yduration = secs;
  ystart_time = cur_time;
  if(ystart_time == 0) ystart_time = SDL_GetTicks();
  }

void SG_AutoScroll::StopYScroll() {
  ystart = 0.0;
  yend = 0.0;
  ystart_time = 0;
  }

