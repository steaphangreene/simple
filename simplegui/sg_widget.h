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

#ifndef SG_WIDGET_H
#define SG_WIDGET_H

#include <SDL/SDL.h>

#define SG_WIDGET_FLAGS_HIDDEN		1
#define SG_WIDGET_FLAGS_DISABLED	2
#define SG_WIDGET_FLAGS_PRESSED		4
#define SG_WIDGET_FLAGS_ON		8

class SG_Widget {
public:
  SG_Widget();
  virtual ~SG_Widget();
  virtual bool HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool HasWidget(SG_Widget *targ);
  virtual bool Render(unsigned long cur_time);
  void Hide() { flags |= SG_WIDGET_FLAGS_HIDDEN; };
  void Show() { flags &= (~SG_WIDGET_FLAGS_HIDDEN); };
  virtual void Disable();
  virtual void Enable();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
//  bool SetCursor(GL_MODEL *cur)
//  bool UnsetCursor()
  
protected:
//  GL_MODEL Mouse_Cursor;
//  static GL_MODEL Default_Mouse_Cursor;
  unsigned long flags;
  };

#endif // SG_WIDGET_H

