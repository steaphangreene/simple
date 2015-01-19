// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SG_ANIMATEDPANEL_H
#define SG_ANIMATEDPANEL_H

#include <string>

using namespace std;

#include "sg_panel.h"

class SG_AnimatedPanel : public SG_Panel {
 public:
  SG_AnimatedPanel(const vector<SimpleTexture> &txtrs, float mspf);
  virtual ~SG_AnimatedPanel();
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);

 protected:
  virtual bool RenderSelf(unsigned long cur_time);
  //  static GL_MODEL Default_Mouse_Cursor;
  float speed;
};

#endif  // SG_ANIMATEDPANEL_H
