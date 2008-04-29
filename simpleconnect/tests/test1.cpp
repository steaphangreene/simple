// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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

int NUM_FRAMES = 32;	//Not #defines so they can be capped later.
int FRAME_DIM = 128;

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

using namespace std;

#include "../../simplegui/simplegui.h"
#include "../../simplevideo/simplevideo.h"
#include "../simpleconnect.h"
#include "audio.h"

#include "click.h"

static SimpleGUI *gui;
static SimpleVideo *video;

int main(int argc, char **argv) {
  char *fontfn = NULL;
  int xs=1024, ys=768;

  video = new SimpleVideo(xs, ys, 4.0/3.0);

  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);

  audio_init(2048);

  int click = audio_buildsound(click_data, sizeof(click_data));

  if(fontfn) gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);

  gui->SetDefaultFontSize(30);

  SG_ScrollingArea *connscr = new SG_ScrollingArea(SG_AUTOSIZE, 12.0);
  SimpleConnect *connector = new SimpleConnect;
  vector<SC_SlotType> slots;
  vector<int> slot_cols;
  slots.push_back(SC_SLOT_PLAYER);
  slot_cols.push_back(1);
  slots.push_back(SC_SLOT_PLAYER);
  slot_cols.push_back(1);
  connector->SetSlots(slots);
  connector->SetSlotColors(slot_cols);
  connector->SetSlotTeams(slot_cols);
  connector->Config();
  connscr->AddWidget(connector);
  gui->MasterWidget()->AddWidget(connscr);

  SDL_Event event;
  int user_quit = 0;
  while(!user_quit) {
    while(gui->PollEvent(&event)) {
      if(event.type == SDL_SG_EVENT) {
	switch(event.user.code) {
	  case(SG_EVENT_SELECT): {
	    audio_play(click, 8, 8);
	    }break;
	  }
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) user_quit = 1;
	else if(event.key.keysym.sym == SDLK_SPACE) {
	  static bool mouseon = true;
	  mouseon = !mouseon;
	  if(mouseon) gui->EnableMouse();
	  else gui->DisableMouse();
	  }
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      }

    video->StartScene();
    gui->RenderStart(SDL_GetTicks());
    gui->RenderFinish(SDL_GetTicks());
    video->FinishScene();
    }
  delete gui;    //Will delete all children

  return 0;
  }
