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

#ifndef SG_WIDGET_H
#define SG_WIDGET_H

#include "SDL.h"

#define SG_ALIGN_LEFT   ST_ALIGN_LEFT
#define SG_ALIGN_CENTER ST_ALIGN_CENTER
#define SG_ALIGN_RIGHT  ST_ALIGN_RIGHT

#define SG_WIDGET_FLAGS_HIDDEN		1
#define SG_WIDGET_FLAGS_DISABLED	2
#define SG_WIDGET_FLAGS_PRESSED		4
#define SG_WIDGET_FLAGS_ON		8
#define SG_WIDGET_FLAGS_IGNORE		16

class SG_AlignmentGeometry;

class SG_Widget {
public:
  SG_Widget();
  virtual ~SG_Widget();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event,
		float x, float y);
  virtual bool HasWidget(SG_Widget *targ);

  virtual void SetAspectRatio(double asp);
  virtual void SetAlignment(int align);

  bool Render(unsigned long cur_time, bool final = false);

  void Hide() { flags |= SG_WIDGET_FLAGS_HIDDEN; };
  void Show() { flags &= (~SG_WIDGET_FLAGS_HIDDEN); };

  void Ignore() { flags |= SG_WIDGET_FLAGS_IGNORE; };
  void Listen() { flags &= (~SG_WIDGET_FLAGS_IGNORE); };

  virtual void Disable();
  virtual void Enable();

  virtual void TurnOn();
  virtual void TurnOff();
  bool IsOn() { return ((flags & SG_WIDGET_FLAGS_ON) > 0); };

//  virtual bool SetDefaultCursor(GL_MODEL *cur);
//  bool SetCursor(GL_MODEL *cur)
//  bool UnsetCursor()

	//Internal Use Only!  For self-reconfiguring widgets.
  virtual void AdjustGeometry(SG_AlignmentGeometry *geom);
  
protected:
  virtual bool RenderSelf(unsigned long cur_time);

//  GL_MODEL Mouse_Cursor;
//  static GL_MODEL Default_Mouse_Cursor;
  unsigned long flags;
  double aspect_ratio;
  };

#endif // SG_WIDGET_H

