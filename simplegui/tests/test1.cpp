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

int NUM_FRAMES = 32;	//Not #defines so they can be capped later.
int FRAME_DIM = 128;

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
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

//  gui = new SimpleGUI(ASPECT_NO_SUPPORT, 4.0/3.0);
  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);
//  gui = new SimpleGUI(ASPECT_EXPANDING_Y|ASPECT_EXPANDING_X, 4.0/3.0);

  gui->LoadFont(fontfn);

  gui->SetDefaultTextColor(0.0, 0.0, 0.0);
  int white = gui->NewColor(1.0, 1.0, 1.0);

  map<SG_Widget *, string> name;

  SG_Table *popup;
  SG_Table *tab[2];
  SG_Panel *panel[3];
  SG_AnimatedPanel *anim;
  SG_Button *button[6];
  SG_Alignment *align[2];
  SG_Scrollable *scroll;
  SG_PassThrough *pass[2];
  SG_TextArea *text;
  SG_TransLabel *trans;
  SG_ProgressBar *prog;

  tab[0] = new SG_Table(4, 12);
  tab[1] = new SG_Table(2, 12);

  scroll = new SG_Scrollable(2.0, 2.0);
  scroll->SetBorder(0.125, 0.125);

  align[0] = new SG_Alignment();
  align[0]->SetBorder(0.0, 0.0);

  align[1] = new SG_Alignment();
  align[1]->SetBorder(0.0, 0.0);

  panel[0] = new SG_Panel(gui->NewColor(0.2, 0.2, 0.2));

  pass[0] = new SG_PassThrough(SG_PT_BOX, SG_PT_CLICK, SG_PT_MENU);
  pass[1] = new SG_PassThrough(SG_PT_BOX, SG_PT_CLICK, SG_PT_CLICK);

  gui->MasterWidget()->AddWidget(tab[0]);
  gui->MasterWidget()->RemoveWidget(tab[0]);	//Just to be sure it works!
  gui->MasterWidget()->AddWidget(tab[0]);

  tab[0]->AddWidget(pass[0], 0, 0, 3, 12);
  tab[0]->AddWidget(tab[1], 3, 0, 1, 12);
  tab[1]->AddWidget(scroll, 0, 0, 2, 4);
  tab[1]->SetBackground(panel[0]);

  tab[0]->AddWidget(align[1], 1, 2, 1, 2);

  trans = new SG_TransLabel("TransLabel Widget\nIs Right Here",
	gui->NewColor(0.0, 0.0, 0.0, 0.6, 0.4, 0.3));
  trans->SetMargins(0.2, 0.2);
  align[1]->AddWidget(trans);

  panel[2] = new SG_Panel(gui->NewColor(0.0, 0.5, 1.0));
  align[1]->SetBackground(panel[2]);

  panel[1] = new SG_Panel(white);

  int maxdim;	// Cap the texture to maximum size for target hardware!
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxdim);
  if(FRAME_DIM > maxdim) FRAME_DIM = maxdim;

  vector<SG_Texture> cols;
  cols.resize(NUM_FRAMES, (SDL_Surface *)(NULL));
  for(int tx=0; tx < NUM_FRAMES; ++tx) {  //Create NUM_FRAMES actual textures
    cols[tx].src = SDL_CreateRGBSurface(0, FRAME_DIM, FRAME_DIM, 32,
	SG_SDL_RGBA_COLFIELDS);
    SDL_FillRect(cols[tx].src, NULL, SDL_MapRGB(cols[tx].src->format, 0, 0, 0));
    unsigned long colwht = SDL_MapRGB(cols[tx].src->format, 255, 255, 255);
    for(int x=0; x < FRAME_DIM; ++x) {
      for(int y=0; y < FRAME_DIM; ++y) {
	if(rand()&32) {
	  ((unsigned long *)(cols[tx].src->pixels))[y*FRAME_DIM+x] = colwht;
	  }
	}
      }
    }
/*
  int black = gui->NewColor(0.0, 0.0, 0.0);
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(white));
  cols.push_back(SG_Texture(black));
  cols.push_back(SG_Texture(white));
*/
  anim = new SG_AnimatedPanel(cols, 12.5f);

  scroll->SetBackground(panel[1]);
  scroll->AddWidget(align[0]);
  align[0]->SetBackground(anim);
  align[0]->AddWidget(pass[1]);

  button[0] = new SG_Button("Red",
	gui->NewColor(1.0, 0.0, 0.0), gui->NewColor(1.0, 0.5, 0.5));
  button[1] = new SG_Button("Green",
	gui->NewColor(0.0, 1.0, 0.0), gui->NewColor(0.5, 1.0, 0.5));
  button[2] = new SG_Button("Blue",
	gui->NewColor(0.0, 0.0, 1.0), gui->NewColor(0.5, 0.5, 1.0));
  button[3] = new SG_StickyButton("Yellow",
	gui->NewColor(1.0, 1.0, 0.0), gui->NewColor(0.5, 0.5, 0.0));
  button[4] = new SG_StickyButton("Cyan",
	gui->NewColor(0.0, 1.0, 1.0), gui->NewColor(0.0, 0.5, 0.5));
  button[5] = new SG_StickyButton("", //Blank!
	gui->NewColor(1.0, 0.0, 1.0), gui->NewColor(0.5, 0.0, 0.5));

  name[button[0]] = "red";
  name[button[1]] = "green";
  name[button[2]] = "blue";
  name[button[3]] = "yellow";
  name[button[4]] = "cyan";
  name[button[5]] = "purple";
  name[pass[0]] = "Main Area";
  name[pass[1]] = "Minimap";

  tab[1]->AddWidget(button[0], 0, 5);
  tab[1]->AddWidget(button[1], 0, 6);
  tab[1]->AddWidget(button[2], 0, 7);
  tab[1]->AddWidget(button[3], 1, 5);
  tab[1]->AddWidget(button[4], 1, 6);
  tab[1]->AddWidget(button[5], 1, 7);

  text = new SG_TextArea(
	"Text Area Is Here", gui->NewColor(0.6, 0.4, 0.3));
  text->SetMargins(0.2, 0.2);
  tab[1]->AddWidget(text, 0, 4, 2, 1);

  prog = new SG_ProgressBar("Progress",
	gui->NewColor(0.3, 0.3, 0.3), gui->NewColor(0.6, 0.0, 0.0));
  prog->SetMargins(0.2, 0.2);
  tab[1]->AddWidget(prog, 0, 8, 2, 1);

//  SG_FileBrowser *fileb;
//  fileb = new SG_FileBrowser("*.cpp");
//  tab[1]->AddWidget(fileb, 0, 9, 2, 3);

  SG_ComboBox *combob;
  vector<string> opts;
  opts.push_back("Option 1");
  opts.push_back("Option 2");
  opts.push_back("Option 3");
  opts.push_back("Option 4");
  combob = new SG_ComboBox(opts);
  tab[1]->AddWidget(combob, 0, 9, 2, 1);

  SG_Alignment *stack1 = new SG_Alignment(0.0, 0.0);
  SG_Button *stack2 = new SG_Button("Click Me");
  SG_TransLabel *stack3 = new SG_TransLabel("Not Me");
  stack1->SetBackground(stack2);
  stack1->AddWidget(stack3);
  tab[1]->AddWidget(stack1, 0, 10, 1, 1);
  stack3->Ignore();

  SG_SliderBar *slide = new SG_SliderBar();
  tab[1]->AddWidget(slide, 0, 11, 2, 1);

  tab[1]->SetBorder(0.0625, 0.125);

  popup = new SG_Table(6, 6);
  popup->SetBackground(new SG_Panel());
//  popup->AddWidget(new SG_Button("Popup!\nHere!"), 1, 1, 4, 4);
  popup->AddWidget(new SG_Editable("Popup!\nHere!"), 1, 1, 4, 4);

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
	  if(event.user.data1 == (void*)button[4])
	    text->SetText("Or is it?");
	  else if(event.user.data1 == (void*)button[5])
	    gui->SetPopupWidget(popup, 0.3, 0.3); 
	  }
	else if(event.user.code == SG_EVENT_STICKYOFF) {
	  printf("Received SG_EVENT_STICKYOFF from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  audio_play(click, 8, 8);
	  if(event.user.data1 == (void*)button[4])
	    text->SetText("Text Area Is Here");
	  else if(event.user.data1 == (void*)button[5])
	    gui->UnsetPopupWidget(); 
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
	else if(event.user.code >= SG_EVENT_MENU
		&& event.user.code < SG_EVENT_MENUMAX) {
	  printf("Received SG_EVENT_BOX from %s, for button #%d"
			" item %d selected.\n",
		name[(SG_Widget*)(event.user.data1)].c_str(),
		event.user.code - SG_EVENT_BOX, *((int*)(event.user.data2)));
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
  delete popup;  //Will delete all children
  delete gui;    //Will delete all children

  return 0;
  }
