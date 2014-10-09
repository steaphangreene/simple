// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SG_AUTOSCROLL_H
#define SG_AUTOSCROLL_H

#include "sg_scrollable.h"

class SG_AutoScroll : public SG_Scrollable {
 public:
  SG_AutoScroll(float xspn, float yspn, float xoff = 0.0, float yoff = 0.0,
                float xmin = 0.0, float ymin = 0.0, float xmax = 1.0,
                float ymax = 1.0);
  virtual ~SG_AutoScroll();

  void SetXScroll(float start, float end, float secs, Uint32 cur_time = 0);
  void StopXScroll();
  void SetYScroll(float start, float end, float secs, Uint32 cur_time = 0);
  void StopYScroll();

 protected:
  virtual bool RenderSelf(unsigned long cur_time);
  float xstart, ystart, xend, yend, xduration, yduration;
  Uint32 xstart_time, ystart_time;
};

#endif  // SG_AUTOSCROLL_H
