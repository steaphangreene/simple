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
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_keysym.h>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../simplegui.h"
#include "renderer.h"
#include "audio.h"

#include "click.h"

static SimpleGUI *gui;

static SG_PullDown *newmenu(const vector<string> &itms) {
  static int num = 0;

  char buf[32] = {0};
  sprintf(buf, "Menu %d", ++num);
  return new SG_PullDown(buf, itms);
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

  int click = audio_buildsound(click_data, sizeof(click_data));

  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);

  gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);

  SG_ScrollingArea *scr = new SG_ScrollingArea(8.0, 16.0, 6.0, 12.0);
  gui->MasterWidget()->AddWidget(scr);

  SG_Table *tab = new SG_Table(8, 16);
  tab->SetBorder(0.0625, 0.125);
  scr->AddWidget(tab);

  map<SG_MultiText*, int> menu2colid;
  map<SG_MultiText*, int> menu2rowid;
  map<int, int> colid2col;
  map<int, int> rowid2row;

  vector<string> opts;
  opts.push_back("Remove Row\n");
  opts.push_back("Remove Col\n");
  opts.push_back("Add Row\n");
  opts.push_back("Add Col\n");

  for(int el = 8*16-1; el>=0; --el) {
    SG_PullDown *menu = newmenu(opts);
    tab->AddWidget(menu, el/16, el%16);
    menu2colid[menu] = el/16;
    menu2rowid[menu] = el%16;
    colid2col[el/16] = el/16;
    rowid2row[el%16] = el%16;
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
	if(event.user.code == SG_EVENT_MENU) {
	  audio_play(click, 8, 8);
	  if(((int *)(event.user.data2))[0] == 0) {		//Remove Row
	    int row = rowid2row[menu2rowid[(SG_MultiText*)event.user.data1]];
	    tab->RemoveRow(row);

	    map<int,int>::iterator itr;
	    for(itr = rowid2row.begin(); itr != rowid2row.end(); ++itr) {
	      if(itr->second > row) --(itr->second);
	      }
	    }
	  else if(((int *)(event.user.data2))[0] == 1) {	//Remove Col
	    int col = colid2col[menu2colid[(SG_MultiText*)event.user.data1]];
	    tab->RemoveCol(col);

	    map<int,int>::iterator itr;
	    for(itr = colid2col.begin(); itr != colid2col.end(); ++itr) {
	      if(itr->second > col) --(itr->second);
	      }
	    }
	  else if(((int *)(event.user.data2))[0] == 2) {	//Add Row
	    int row = rowid2row[menu2rowid[(SG_MultiText*)event.user.data1]];
	    tab->AddRow(row);

	    map<int,int>::iterator itr;
	    for(itr = rowid2row.begin(); itr != rowid2row.end(); ++itr) {
	      if(itr->second >= row) ++(itr->second);
	      }
	    }
	  else {						//Add Col
	    int col = colid2col[menu2colid[(SG_MultiText*)event.user.data1]];
	    tab->AddCol(col);

	    map<int,int>::iterator itr;
	    for(itr = colid2col.begin(); itr != colid2col.end(); ++itr) {
	      if(itr->second >= col) ++(itr->second);
	      }
	    }
	  }
	}
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) user_quit = 1;
	}
      else if(event.type == SDL_QUIT) {
	user_quit = 1;
	}
      } while(gui->PollEvent(&event));
    start_scene();
    gui->Render(SDL_GetTicks());
    finish_scene();
    }

  delete gui;    //Will delete all children

  return 0;
  }
