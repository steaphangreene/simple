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

//Test 3 is a copy of Test 1 modified to test SG_ListBox
//rob w. brooks (rob@atomicpenguin.net)

int NUM_FRAMES = 32;	//Not #defines so they can be capped later.
int FRAME_DIM = 128;

#include <SDL/SDL.h>
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
#include "bong.h"

static SimpleGUI *gui;

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

  int click = audio_buildsound(click_data, sizeof(click_data));
  int bong = audio_buildsound(bong_data, sizeof(bong_data));

  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);

  gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);
  int white = gui->NewColor(1.0, 1.0, 1.0);

  map<SG_Widget *, string> name;

  SG_Table *tab;
  SG_ListBox *listb;
  SG_ListBox *hlistb;

  tab = new SG_Table(10, 10);

  gui->MasterWidget()->AddWidget(tab);

  vector<SG_Widget*> items;

  int col=gui->NewColor(0,0,0,0.5,0.5,1.0);
  int blue=gui->NewColor(0.0,0.0,1.0);
  int dkblue=gui->NewColor(0,0,0.6);
  int black=gui->NewColor(0,0,0);
  int red = gui->NewColor(1,0,0);
  int dkgray=gui->NewColor(0.2,0.2,0.2);
  int gray=gui->NewColor(0.5,0.5,0.5);

  items.push_back(new SG_TransLabel("Item 1",col));
  items.push_back(new SG_TransLabel("Item 2",col));
  items.push_back(new SG_TransLabel("Item 3",col));		  
  listb = new SG_ListBox(items,white,dkblue,blue,black,1,2);

  tab->AddWidget(listb,0,0,4,4);

  vector<SG_Widget*> horzitems;

  horzitems.push_back(new SG_TransLabel("H1",col));
  horzitems.push_back(new SG_TransLabel("H2",col));
  horzitems.push_back(new SG_TransLabel("H3",col));
  horzitems.push_back(new SG_TransLabel("H4",col));
  horzitems.push_back(new SG_TransLabel("H5",col));
  horzitems.push_back(new SG_TransLabel("H6",col));

  hlistb = new SG_ListBox(horzitems,dkgray,red,gray,black,2,4,false);

  tab->AddWidget(hlistb,0,6,10,2);
  
  SDL_Event event;
  int user_quit = 0;
  while(!user_quit) {
    while(SDL_PollEvent(&event)) {
      if(!gui->ProcessEvent(&event)) continue;
      if(event.type == SDL_SG_EVENT) {
	if(event.user.code == SG_EVENT_BUTTONCLICK) {
	  printf("Received SG_EVENT_BUTTONCLICK from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  }
	else if(event.user.code == SG_EVENT_BUTTONPRESS) {
	  printf("Received SG_EVENT_BUTTONPRESS from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_STICKYON) {
	  printf("Received SG_EVENT_STICKYON from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_STICKYOFF) {
	  printf("Received SG_EVENT_STICKYOFF from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_BUTTONRELEASE) {
	  printf("Received SG_EVENT_BUTTONRELEASE from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  }
	else if(event.user.code == SG_EVENT_LEFTCLICK) {
	  printf("Received SG_EVENT_LEFTCLICK from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_MIDDLECLICK) {
	  printf("Received SG_EVENT_MIDDLECLICK from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_RIGHTCLICK) {
	  printf("Received SG_EVENT_RIGHTCLICK from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_SCROLLUP) {
	  printf("Received SG_EVENT_SCROLLUP from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  }
	else if(event.user.code == SG_EVENT_SCROLLDOWN) {
	  printf("Received SG_EVENT_SCROLLDOWN from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  }
	else if(event.user.code >= SG_EVENT_CLICK
		&& event.user.code < SG_EVENT_CLICKMAX) {
	  printf("Received SG_EVENT_OTHERCLICK from %s, at (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code >= SG_EVENT_BOX
		&& event.user.code < SG_EVENT_BOXMAX) {
	  printf("Received SG_EVENT_BOX from %s, for button #%d"
			" - for area (%f,%f) - (%f,%f).\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		event.user.code - SG_EVENT_BOX,
		((float*)(event.user.data2))[2],
		((float*)(event.user.data2))[3],
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_FILEOPEN) {
	  printf("Received SG_EVENT_FILEOPEN for filename '%s'.\n",
		((SG_FileBrowser*)(event.user.data1))->FileName().c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_OK) {
	  printf("Received SG_EVENT_OK from a compound widget.\n");
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_EDIT) {
	  printf("Received SG_EVENT_EDIT - value = '%s'.\n",
		((SG_Editable*)(event.user.data1))->Text().c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_EDITABORT) {
	  printf("Received SG_EVENT_EDITABORT - value = '%s'.\n",
		((SG_Editable*)(event.user.data1))->Text().c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_EDITCOMPLETE) {
	  printf("Received SG_EVENT_EDITCOMPLETE - value = '%s'.\n",
		((SG_Editable*)(event.user.data1))->Text().c_str());
	  audio_play(click, 8, 8);
	  }
	else {
	  printf("Received Unknown SG_EVENT #%d.\n", event.user.code);
	  }
	}
      else if(event.type == SDL_MOUSEBUTTONDOWN) {
	printf("Received unintercepted mouse button press.\n");
	}
      else if(event.type == SDL_MOUSEBUTTONUP) {
//	printf("Received unintercepted mouse button release.\n");
	}
      else if(event.type == SDL_MOUSEMOTION) {
//	printf("Received unintercepted mouse motion.\n");
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) user_quit = 1;
	else {
//	  printf("Received unintercepted key press.\n");
	  }
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      else if(event.type == SDL_KEYUP) {
//	printf("Received unintercepted key release.\n");
	}
      }
    start_scene();
    gui->Render(SDL_GetTicks());
    finish_scene();
    }

  gui->UnsetPopupWidget(); 
  delete gui;    //Will delete all children

  return 0;
  }
