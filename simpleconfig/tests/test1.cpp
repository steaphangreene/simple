// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "../simpleconfig.h"
#include "renderer.h"
#include "audio.h"

#include "click.h"

static SimpleGUI *gui;

int main(int argc, char **argv) {
  char *fontfn = "fonts/Adventure Subtitles Normal.ttf";
  int xs=1024, ys=768;

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }
  audio_init(2048);

  int click = audio_buildsound(click_data, sizeof(click_data));

  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);

  gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);

  gui->MasterWidget()->AddWidget(new SimpleConfig);

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

    start_scene();
    gui->RenderStart(SDL_GetTicks());
    gui->RenderFinish(SDL_GetTicks());
    finish_scene();
    }
  delete gui;    //Will delete all children

  return 0;
  }
