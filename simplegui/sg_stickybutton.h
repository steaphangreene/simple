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

#ifndef SG_STICKYBUTTON_H
#define SG_STICKYBUTTON_H

#include "sg_button.h"

class SG_StickyButton : public SG_Button {
 public:
  SG_StickyButton(const string mes = "", SimpleTexture tex = SG_COL_RAISED,
                  SimpleTexture dis_tex = SG_COL_LOW,
                  SimpleTexture click_tex = SG_COL_LOW,
                  SimpleTexture down_tex = SG_COL_HIGH);
  virtual ~SG_StickyButton();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual void TurnOn();
  virtual void TurnOff();

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;
};

#endif  // SG_STICKYBUTTON_H
