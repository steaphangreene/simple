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

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_keysym.h>
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
SDL_mutex *gui_mutex;

int event_thread_handler(void *arg) {
  int click = audio_buildsound(click_data, sizeof(click_data));

  SDL_Event event;

  while(!user_quit && SDL_WaitEvent(&event)) { {
      int have_event = 0;

      SDL_mutexP(gui_mutex);
      have_event = gui->ProcessEvent(&event);
      SDL_mutexV(gui_mutex);

      if(!have_event) continue;
      if(event.type == SDL_SG_EVENT) {
	if(event.user.code == SG_EVENT_BUTTONPRESS) {
	  audio_play(click, 8, 8);
	  }
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) {
	  user_quit = 1;
	  }
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      }
    }
  return 0;
  }

int video_thread_handler(void *arg) {
  while(!user_quit) {

    start_scene(0);
    unsigned long cur_time = SDL_GetTicks();

    SDL_mutexP(gui_mutex);
    gui->RenderStart(cur_time);
    SDL_mutexV(gui_mutex);

    SDL_mutexP(gui_mutex);
    gui->Render(cur_time);
    SDL_mutexV(gui_mutex);

    finish_scene(0);
    }
  return 0;
  }

int game_thread_handler(void *arg) {
  while(!user_quit) {

//  Do game stuff - carfully coordinated with input from events
//  and output to renderers

//  This would just busy-wait as it's called now, so I don't really call it

    }
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

  SG_Table *btab = new SG_Table(3, 12, 0.0625, 0.0625);
  split->AddWidget(btab, 3, 0);

  SG_Panel *btab_bg = new SG_Panel;
  btab->SetBackground(btab_bg);

  for(int n=0; n < 3*12; ++n) {
    static char buf[8] = { 0 };
    sprintf(buf, "B:%d%c", n+1, 0);  //I don't trust sprintf() to null term.
    btab->AddWidget(new SG_Button(buf));
    }

  gui_mutex = SDL_CreateMutex();

  SDL_Thread *ev_t, *rend_t;

  ev_t = SDL_CreateThread(event_thread_handler, NULL);
  rend_t = SDL_CreateThread(video_thread_handler, NULL);
//  game_thread_handler(NULL); //THIS THREAD would be the game handling thread

  SDL_WaitThread(ev_t, NULL);
  SDL_WaitThread(rend_t, NULL);

  delete gui;
  return 0;
  }
