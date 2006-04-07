// *************************************************************************
//  This file is part of the SimpleConfig Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleConfig is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleConfig is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleConfig (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL_opengl.h"

#include "simpleconfig.h"
#include "../simplegui/simplegui.h"
#include "../simplevideo/simplevideo.h"

SimpleConfig::SimpleConfig(const vector<string> &other_tabs,
        const vector<SG_Alignment*> &other_screens) {
  vector<string> items(4 + other_tabs.size());
  vector<SG_Alignment*> screens(4 + other_tabs.size());
  items[0] = "Video";
  items[1] = "Audio";
  items[2] = "Mouse";
  items[3] = "Keyboard";
  screens[0] = BuildVideoScreen();
  screens[1] = BuildAudioScreen();
  screens[2] = BuildMouseScreen();
  screens[3] = BuildKeyboardScreen();
  copy(other_tabs.begin(), other_tabs.end(), items.begin()+4);
  copy(other_screens.begin(), other_screens.end(), screens.begin()+4);
  Resize(1, 16);
  SetItems(items, screens);
  }

SimpleConfig::~SimpleConfig() {
  }

bool SimpleConfig::Render(unsigned long cur_time) {
  return SG_MultiTab::Render(cur_time);
  }

bool SimpleConfig::ChildEvent(SDL_Event *event) {
  return SG_MultiTab::ChildEvent(event);
  }

//  bool SimpleConfig::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SimpleConfig::Default_Mouse_Cursor = NULL;

SG_Alignment *SimpleConfig::BuildVideoScreen() {
  SG_Table *ret;
  ret = new SG_Table(2, 15);
  ret->SetBorder(0.0, 0.0);

  SG_TextArea *label = new SG_TextArea("Video Config");
  ret->AddWidget(label, 0, 0, 2, 2);

  vector<SimpleVideo_Mode> modes = SimpleVideo::CurrentVideo()->GetModes();
  vector<string> modenames;
  modenames.push_back("Resizable Window");
  Uint32 lastx = 0, lasty = 0;
  for(Uint32 mode = 0; mode < modes.size(); ++mode) {
    if(modes[mode].x == lastx && modes[mode].y == lasty) continue;
    lastx = modes[mode].x;
    lasty = modes[mode].y;
    char buf[256];
    sprintf(buf, "%dx%d Fullscreen%c", modes[mode].x, modes[mode].y, 0);
    modenames.push_back(buf);
    }

  SG_TextArea *mlabel = new SG_TextArea("Resolution:");
  ret->AddWidget(mlabel, 0, 2);
  SG_ComboBox *modebox = new SG_ComboBox(modenames);
  ret->AddWidget(modebox, 1, 2);

  return (SG_Alignment *)ret;
  }

SG_Alignment *SimpleConfig::BuildAudioScreen() {
  SG_Table *ret;
  ret = new SG_Table(2, 15);
  ret->SetBorder(0.0, 0.0);
  SG_TextArea *label = new SG_TextArea("Audio Config");
  ret->AddWidget(label, 0, 0, 2, 2);
  return (SG_Alignment *)ret;
  }

SG_Alignment *SimpleConfig::BuildMouseScreen() {
  SG_Table *ret;
  ret = new SG_Table(2, 15);
  ret->SetBorder(0.0, 0.0);
  SG_TextArea *label = new SG_TextArea("Mouse Config");
  ret->AddWidget(label, 0, 0, 2, 2);
  return (SG_Alignment *)ret;
  }

SG_Alignment *SimpleConfig::BuildKeyboardScreen() {
  SG_Table *ret;
  ret = new SG_Table(2, 15);
  ret->SetBorder(0.0, 0.0);
  SG_TextArea *label = new SG_TextArea("Keyboard Config");
  ret->AddWidget(label, 0, 0, 2, 2);
  return (SG_Alignment *)ret;
  }
