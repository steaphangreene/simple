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

#ifndef SG_BUTTON_H
#define SG_BUTTON_H

#include "sg_textarea.h"

class SG_Button : public SG_TextArea {
 public:
  SG_Button(const string &mes = "", SimpleTexture tex = SG_COL_RAISED,
            SimpleTexture dis_tex = SG_COL_LOW,
            SimpleTexture click_tex = SG_COL_LOW);
  virtual ~SG_Button();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);

 protected:
  //  static GL_MODEL Default_Mouse_Cursor;

  static ST_Texturator *stt_butt_up;
  static ST_Texturator *stt_butt_dn;
};

#endif  // SG_BUTTON_H
