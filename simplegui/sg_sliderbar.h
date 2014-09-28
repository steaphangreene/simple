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

// This file was created from (or actually IS) a basic compound widget
// def, so it's not defined and is really just a place-holder for now.

#ifndef SG_SLIDERBAR_H
#define SG_SLIDERBAR_H

#include "sg_compound.h"
#include "simpletexture.h"
#include "sg_colors.h"
#include "sg_ranger.h"

class SG_Button;
class SG_TextArea;
class SG_Dragable;

class SG_SliderBar : public SG_Compound, public SG_Ranger {
public:
  SG_SliderBar(bool vert = false,
        SimpleTexture b1tex = SG_COL_RAISED, SimpleTexture b1tex_dis = SG_COL_LOW,
        SimpleTexture b1tex_click = SG_COL_LOW,
        SimpleTexture b2tex = SG_COL_RAISED, SimpleTexture b2tex_dis = SG_COL_LOW,
        SimpleTexture b2tex_click = SG_COL_LOW,
	SimpleTexture handtex = SG_COL_FG, SimpleTexture bgtex = SG_COL_BG);
  virtual ~SG_SliderBar();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool ChildEvent(SDL_Event *event);

protected:
  virtual bool RenderSelf(unsigned long cur_time);

  static ST_Texturator *stt_slider;

//  static GL_MODEL Default_Mouse_Cursor;
  SG_Dragable *handle;
  SG_Button *incb, *decb;
  bool vertical;
  };

#endif // SG_SLIDERBAR_H
