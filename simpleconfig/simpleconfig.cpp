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
#include "simplegui.h"
#include "simplevideo.h"

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
  SetItems(items);
  SetAreas(screens);
  mode = 0;
  oldmode = 0;
  timeleft = 0;
  disptime = 0;
  setback = false;
  confirm = true;
  rescue_thread = NULL;
  SetBackground(new SG_Panel());
  }

SimpleConfig::~SimpleConfig() {
  confirm = true;
  if(rescue_thread) SDL_WaitThread(rescue_thread, NULL);
  rescue_thread = NULL;
  }

bool SimpleConfig::RenderSelf(unsigned long cur_time) {
  if(setback) {
    SetMode(oldmode);
    setback = false;
    modebox->Set(mode);
    }
  if(timeleft != disptime) {
    if(disptime == 0) {
      modebox->Hide();
      modebox->Ignore();
      rescue_label->Hide();
      rescue_indicator->Show();
      confirmbut->Show();
      confirmbut->Listen();
      }
    else if(timeleft == 0) {
      confirmbut->Ignore();
      confirmbut->Hide();
      rescue_indicator->Hide();
      rescue_label->Show();
      modebox->Show();
      modebox->Listen();
      }
    disptime = timeleft;
    if(disptime != 0) {
      char buf[64];
      sprintf(buf, "Confirm in %d seconds or will be undone%c",
	(disptime+999)/1000, 0);
      rescue_indicator->SetText(buf);
      }
    }
  return SG_MultiTab::RenderSelf(cur_time);
  }

bool SimpleConfig::ChildEvent(SDL_Event *event) {
  if(event->type == SDL_SG_EVENT) {
    switch(event->user.code) {
      case(SG_EVENT_NEWTEXT): {
	oldmode = mode;
	mode = -1;
	for(int ctr=0; ctr < modebox->NumItems(); ++ctr) {
	  if(modebox->Text() == modebox->Item(ctr)) {
	    SetMode(ctr);
	    break;
	    }
	  }
	if(mode == -1) {
	  fprintf(stderr, "ERROR, Got a non-mode mode!\n");
	  exit(1);
	  }
	if(rescue_thread) {
	  confirm = true;
	  SDL_WaitThread(rescue_thread, NULL);
	  rescue_thread = NULL;
	  }
	confirm = false;
	rescue_thread = SDL_CreateThread(rescue_thread_handler, (void*)(this));
	}break;
      case(SG_EVENT_BUTTONCLICK): {
	if((SG_Widget*)(event->user.data1) == (SG_Widget*)confirmbut) {
	  confirm = 1;
	  }
	}break;
      }
    }
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

  xsize.clear();
  ysize.clear();
  vector<SimpleVideo_Mode> modes =
	SimpleVideo::CurrentVideo()->GetFullScreenModes();
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
    xsize.push_back(int(modes[mode].x));
    ysize.push_back(int(modes[mode].y));
    }

  SG_TextArea *mlabel = new SG_TextArea("Resolution:");
  ret->AddWidget(mlabel, 0, 2);

  confirmbut = new SG_Button("Confirm Change");
  confirmbut->Hide();
  confirmbut->Ignore();
  ret->AddWidget(confirmbut, 1, 2);

  modebox = new SG_ComboBox(modenames);
  ret->AddWidget(modebox, 1, 2);

  rescue_indicator = new SG_TextArea("");
  rescue_indicator->Hide();
  ret->AddWidget(rescue_indicator, 0, 3, 2, 1);

  rescue_label =
	new SG_TextArea("You will have 5 seconds to confirm changes");
  ret->AddWidget(rescue_label, 0, 3, 2, 1);

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

int SimpleConfig::HandleRescueThread() {
  Uint32 ttime = 5000;	// 5 Seconds
  Uint32 tstep = 250;	// In 1/4 second steps
  for(timeleft = ttime; timeleft > 0 && (!confirm); timeleft -= tstep) {
    SDL_Delay(tstep);
    }
  if(!confirm) {
    setback = true;
    }
  else {
    confirm = false;
    }
  timeleft = 0;
  return 0;
  }

int SimpleConfig::rescue_thread_handler(void *me) {
  return ((SimpleConfig*)(me))->HandleRescueThread();
  }

void SimpleConfig::SetMode(int md) {
  mode = md;
  if(mode == 0) {
    SimpleVideo::CurrentVideo()->SetWindowedMode(1024, 768);	//FIXME: Size!
    }
  else {
    SimpleVideo::CurrentVideo()->SetFullScreenMode(
	xsize[mode-1], ysize[mode-1]
	);
    }
  }
