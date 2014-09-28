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

#ifndef SG_COMPOUND_H
#define SG_COMPOUND_H

#include "sg_table.h"
#include "simpletexture.h"

class SG_Compound : public SG_Table {
public:
  SG_Compound(int xsz, int ysz, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Compound();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

protected:
//  static GL_MODEL Default_Mouse_Cursor;

  static ST_Texturator *stt_upbutt_up;
  static ST_Texturator *stt_upbutt_dn;
  static ST_Texturator *stt_dnbutt_up;
  static ST_Texturator *stt_dnbutt_dn;
  static ST_Texturator *stt_rtbutt_up;
  static ST_Texturator *stt_rtbutt_dn;
  static ST_Texturator *stt_ltbutt_up;
  static ST_Texturator *stt_ltbutt_dn;
  };

#endif // SG_COMPOUND_H

