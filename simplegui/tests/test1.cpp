#include <SDL/SDL.h>
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
  int xs=1024, ys=768;
  if(argc > 1) {
    if(sscanf(argv[1], "%dx%d", &xs, &ys) != 2) {
      xs=1024; ys=768;
      }
    }
  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }
  audio_init(256);

  int click = audio_buildsound(click_data, sizeof(click_data));
  int bong = audio_buildsound(bong_data, sizeof(bong_data));

  gui = new SimpleGUI(ASPECT_NO_SUPPORT, 4.0/3.0);
//  gui = new SimpleGUI(ASPECT_FIXED_Y|ASPECT_FIXED_X, 4.0/3.0);
//  gui = new SimpleGUI(ASPECT_EXPANDING_Y|ASPECT_EXPANDING_X, 4.0/3.0);


  map<SG_Widget *, string> name;

  SG_Table *tab[2];
  SG_Widget *widget[3];
  SG_Button *button[6];
  SG_Alignment *align;
  SG_PassThrough *pass;
  SG_TextArea *text;

  tab[0] = new SG_Table(4, 1);
  tab[1] = new SG_Table(2, 12);

  align = new SG_Alignment();
  align->SetBorder(0.25, 0.25);

  widget[0] = new SG_Panel(0.2, 0.2, 0.2);

  pass = new SG_PassThrough(SG_PT_BOX, SG_PT_CLICK, SG_PT_CLICK);

  gui->MasterWidget()->AddWidget(tab[0]);
  gui->MasterWidget()->RemoveWidget(tab[0]);	//Just to be sure it works!
  gui->MasterWidget()->AddWidget(tab[0]);
  tab[0]->AddWidget(pass, 0, 0, 3, 1);
  tab[0]->AddWidget(tab[1], 3, 0);
  tab[1]->AddWidget(align, 0, 0, 2, 4);
  tab[1]->SetBackground(widget[0]);

  widget[1] = new SG_Panel(1.0, 1.0, 1.0);
  widget[2] = new SG_Panel(0.0, 0.0, 0.0);

  align->SetBackground(widget[1]);
  align->AddWidget(widget[2]);

  button[0] = new SG_Button("Red",
		1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5);
  button[1] = new SG_Button("Green",
		0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 0.5);
  button[2] = new SG_Button("Blue",
		0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.5, 1.0);
  button[3] = new SG_Button("Yellow",
		1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5, 0.0);
  button[4] = new SG_Button("Cyan",
		0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.5, 0.5);
  button[5] = new SG_Button("Purple",
		1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.5);

  name[button[0]] = "red";
  name[button[1]] = "green";
  name[button[2]] = "blue";
  name[button[3]] = "yellow";
  name[button[4]] = "cyan";
  name[button[5]] = "purple";

  tab[1]->AddWidget(button[0], 0, 5);
  tab[1]->AddWidget(button[1], 0, 6);
  tab[1]->AddWidget(button[2], 0, 7);
  tab[1]->AddWidget(button[3], 1, 5);
  tab[1]->AddWidget(button[4], 1, 6);
  tab[1]->AddWidget(button[5], 1, 7);

  text = new SG_TextArea("Text Area Aquired", 0.6, 0.4, 0.3, 0.0, 0.0, 0.0);
  text->SetMargins(0.2, 0.2);
  tab[1]->AddWidget(text, 0, 4, 2, 1);

  tab[1]->SetBorder(0.0625, 0.125);

  SDL_Event event;
  int user_quit = 0;
  while(!user_quit) {
    while(SDL_PollEvent(&event)) {
      if(!gui->ProcessEvent(&event)) continue;
      if(event.type == SDL_SG_EVENT) {
	if(event.user.code == SG_EVENT_BUTTONPRESS) {
	  printf("Received SG_EVENT_BUTTONPRESS from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  audio_play(click, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_BUTTONRELEASE) {
	  printf("Received SG_EVENT_BUTTONRELEASE from %s button.\n",
		name[(SG_Widget*)(event.user.data1)].c_str());
	  }
	else if(event.user.code == SG_EVENT_LEFTCLICK) {
	  printf("Received SG_EVENT_LEFTCLICK from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_MIDDLECLICK) {
	  printf("Received SG_EVENT_MIDDLECLICK from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_RIGHTCLICK) {
	  printf("Received SG_EVENT_RIGHTCLICK from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code == SG_EVENT_SCROLLUP) {
	  printf("Received SG_EVENT_SCROLLUP from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  }
	else if(event.user.code == SG_EVENT_SCROLLDOWN) {
	  printf("Received SG_EVENT_SCROLLDOWN from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  }
	else if(event.user.code >= SG_EVENT_CLICK
		&& event.user.code < SG_EVENT_CLICKMAX) {
	  printf("Received SG_EVENT_OTHERCLICK from PassThrough, at (%f,%f).\n",
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
	  }
	else if(event.user.code >= SG_EVENT_BOX
		&& event.user.code < SG_EVENT_BOXMAX) {
	  printf("Received SG_EVENT_BOX from PassThrough, for button #%d"
			" - for area (%f,%f) - (%f,%f).\n",
		event.user.code - SG_EVENT_BOX,
		((float*)(event.user.data2))[2],
		((float*)(event.user.data2))[3],
		((float*)(event.user.data2))[0],
		((float*)(event.user.data2))[1]);
	  audio_play(bong, 8, 8);
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
    start_scene(0);
    gui->Render(SDL_GetTicks());
    finish_scene(0);
    }

  return 0;
  }
