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

#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_ttf.h"
#include "SDL_keysym.h"
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

using namespace std;

#include "../simplegui.h"
#include "renderer.h"
#include "audio.h"

#include "click.h"

int user_quit = 0;	//Don't need to mutex this one - no race condition
static SimpleGUI *gui;

static bool go_left = false, go_right = false, go_up = false, go_down = false;
static SG_Tabs *tabs = NULL;

int event_thread_handler(void *arg) {
  int click = audio_buildsound(click_data, sizeof(click_data));

  SDL_Event event;

  while(!user_quit) {
    while(!user_quit && gui->WaitEvent(&event, true)) {
      if(event.type == SDL_SG_EVENT) {
	if(event.user.code == SG_EVENT_BUTTONPRESS) {
	  audio_play(click, 8, 8);
	  }
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) {
	  user_quit = 1;
	  }
	else if(event.key.keysym.sym == SDLK_LEFT) {
	  while(go_left) SDL_Delay(10);
	  go_left = true;
	  }
	else if(event.key.keysym.sym == SDLK_RIGHT) {
	  while(go_right) SDL_Delay(10);
	  go_right = true;
	  }
	else if(event.key.keysym.sym == SDLK_UP) {
	  while(go_up) SDL_Delay(10);
	  go_up = true;
	  }
	else if(event.key.keysym.sym == SDLK_DOWN) {
	  while(go_down) SDL_Delay(10);
	  go_down = true;
	  }
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      }
    SDL_Delay(10); // Weak, yes, but this is how SDL does it too. :(
    }
  return 0;
  }

int video_thread_handler(void *arg) { // MUST BE IN SAME THREAD AS SDL_Init()!
  while(!user_quit) {
    unsigned long cur_time = SDL_GetTicks();

    if(go_left)  { tabs->Left();  go_left = false;  }
    if(go_right) { tabs->Right(); go_right = false; }
    if(go_up)    { tabs->Up();    go_up = false;    }
    if(go_down)  { tabs->Down();  go_down = false;  }

    start_scene();
    gui->RenderStart(cur_time, true);
    gui->RenderFinish(cur_time, true);
    finish_scene();
    }
  return 0;
  }

int game_thread_handler(void *arg) {
//  while(!user_quit) {
//    We would do game stuff - carfully coordinated with input from events
//    and output to renderers.

//    This would just busy-wait as it's written now, so the while() is
//    commented out.
//    }
  return 0;
  }

int main(int argc, char **argv) {
  char *fontfn = "fonts/Adventure Subtitles Normal.ttf";
  int xs=640, ys=480;

  int cur_arg = 1;

  while(1) {		//Don't ever code anything like this
    if(argc > cur_arg) {
      if(!strcasecmp(argv[cur_arg]+strlen(argv[cur_arg])-4, ".ttf")) {
	fontfn = argv[cur_arg];
	++cur_arg;
	continue;
	}	
      }
    if(argc > cur_arg) {
      if(sscanf(argv[cur_arg], "%dx%d", &xs, &ys) != 2) {
	xs=1024; ys=768;
	}
      else {
	++cur_arg;
	continue;
	}
      }
    break;
    }

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }
  audio_init(2048);

//  gui = new SimpleGUI(ASPECT_NO_SUPPORT, 4.0/3.0);
  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);
//  gui = new SimpleGUI(ASPECT_EXPANDING_Y|ASPECT_EXPANDING_X, 4.0/3.0);

  gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);

  SG_Table *split = new SG_Table(4, 1);
  gui->MasterWidget()->AddWidget(split);

  vector<string> buttons;
  for(int n=0; n < 4*16; ++n) {
    static char buf[8] = { 0 };
    sprintf(buf, "B:%d%c", n+1, 0);  //I don't trust sprintf() to null term.
    buttons.push_back(buf);
    }
  SG_Scrollable *scr = new SG_Scrollable(3.0, 12.0);
  split->AddWidget(scr, 3, 0);

  tabs = new SG_Tabs(buttons, 4, 16);
  tabs->SetBorder(0.0625, 0.0625);
  scr->AddWidget(tabs);

  SG_Panel *btab_bg = new SG_Panel;
  tabs->SetBackground(btab_bg);
  
  SDL_Thread *ev_t, *game_t;

  ev_t = SDL_CreateThread(event_thread_handler, NULL);
  game_t = SDL_CreateThread(game_thread_handler, NULL);
  video_thread_handler(NULL); //THIS thread MUST be the video thread

  SDL_WaitThread(ev_t, NULL);
  SDL_WaitThread(game_t, NULL);

  delete gui;
  return 0;
  }
