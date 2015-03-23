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

#ifndef SG_PANEL_H
#define SG_PANEL_H

#include "SDL_opengl.h"

#include <vector>

using namespace std;

#include "sg_colors.h"
#include "simpletexture.h"
#include "sg_widget.h"

#define SG_STATE_DEFAULT 0
#define SG_STATE_DISABLED 1

class SG_Panel : public SG_Widget {
 public:
  SG_Panel(SimpleTexture tex = SG_COL_BG);
  virtual ~SG_Panel();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  void SetTransparent(bool val = true);

  void SetTexture(SimpleTexture tex, int st = SG_STATE_DEFAULT);
  void SetTexturator(ST_Texturator *stt, int st = SG_STATE_DEFAULT);

  // For Internal Use Only!
  void SetState(int st) { state = st; };
  int State() { return state; };

  virtual void SetAspectRatio(float asp);

 protected:
  virtual bool RenderSelf(unsigned long cur_time);
  //  static GL_MODEL Default_Mouse_Cursor;

  vector<SimpleTexture> texture;
  int state;  // Which texture to use - for multi-state support
};

#endif  // SG_PANEL_H
