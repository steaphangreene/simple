// *************************************************************************
//  This file is part of the SimpleConfig Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleConfig is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleConfig is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConfig (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef SIMPLECONFIG_H
#define SIMPLECONFIG_H

#include <string>
#include <vector>
using namespace std;

#include "SDL_thread.h"
#include "sg_multitab.h"

class SG_Button;
class SG_TextArea;
class SG_ComboBox;
class SG_Alignment;

class SimpleConfig : public SG_MultiTab {
 public:
  SimpleConfig(
      const vector<string> &other_tabs = vector<string>(),
      const vector<SG_Alignment *> &other_screens = vector<SG_Alignment *>());
  virtual ~SimpleConfig();

  //  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);

 protected:
  virtual bool RenderSelf(unsigned long cur_time);

  //  static GL_MODEL Default_Mouse_Cursor;
  SG_Alignment *BuildVideoScreen();
  SG_Alignment *BuildAudioScreen();
  SG_Alignment *BuildMouseScreen();
  SG_Alignment *BuildKeyboardScreen();

  static int rescue_thread_handler(void *me);
  int HandleRescueThread();
  SDL_Thread *rescue_thread;
  bool setback, confirm;
  Uint32 timeleft, disptime;

  void SetMode(int md);
  int mode, oldmode;
  SG_ComboBox *modebox;
  SG_Button *confirmbut;
  SG_TextArea *rescue_indicator;
  SG_TextArea *rescue_label;
  vector<int> xsize, ysize;
};

#endif  // SIMPLECONFIG_H
