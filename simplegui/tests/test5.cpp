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

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_ttf.h"
#include "SDL_keysym.h"
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../simplegui.h"
#include "simplevideo.h"

static SimpleVideo *video;
static SimpleGUI *gui;

int main(int argc, char **argv) {
  char *fontfn = NULL;
  int xs=768, ys=480;

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

  video = new SimpleVideo(1235, 650, 0.0);

  gui = new SimpleGUI(ASPECT_DYNAMIC, 19.0/10.0);

  if(fontfn) gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(1.0, 1.0, 1.0);

  SG_AspectTable *main = new SG_AspectTable(19.0/10.0, 1235.0, 650.0, 0.0, 0.0);
  gui->MasterWidget()->AddWidget(main);

  { SG_Panel *pan = NULL;
    pan = new SG_TextArea("USA", SimpleTexture("blue"));
    main->AddWidget(pan, 0, 0, 741, 350);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 494, 0, 741, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 494, 50, 741, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 494, 100, 741, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 494, 150, 741, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 494, 200, 741, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 494, 250, 741, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 494, 300, 741, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 0, 350, 1235, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 0, 400, 1235, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 0, 450, 1235, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 0, 500, 1235, 50);
    pan = new SG_Panel(SimpleTexture("white"));
    main->AddWidget(pan, 0, 550, 1235, 50);
    pan = new SG_Panel(SimpleTexture("red"));
    main->AddWidget(pan, 0, 600, 1235, 50);
    }

  SDL_Event event;
  int user_quit = 0;
  while(!user_quit) {
//    while(gui->PollEvent(&event)) {
    gui->WaitEvent(&event);
    do {
      if(event.type == SDL_SG_EVENT) {
	if(event.user.code == SG_EVENT_MOVE) {
	  printf("Got SG_EVENT_MOVE to %f\n", ((float*)(event.user.data2))[0]);
	  }
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) user_quit = 1;
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      } while(gui->PollEvent(&event));
    video->StartScene();
    gui->Render(SDL_GetTicks());
    video->FinishScene();
    }

  delete gui;    //Will delete all children

  return 0;
  }
