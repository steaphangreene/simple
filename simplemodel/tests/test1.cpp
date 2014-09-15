// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_keysym.h"

#include <cstdio>
#include <string>

using namespace std;

#include "../simplemodel.h"
#include "renderer.h"

static vector<int> anims;
static vector<Uint32> times;

static vector<SimpleModel *> mod;
static Uint32 modnum = 0;
static SimpleModel *weap = NULL;

static SimpleTexture *banner = NULL;

static bool verbose = false;

static void SetAnim(int which, string anim_name = "") {
  static int anim = 0;
  int oldanim = anim;

  if(which == -1) ++anim;
  else if(which == -2) --anim;

  map<string, int> anim_map = mod[modnum]->GetAnimations();
  if(which < 0) {
    map<string, int>::const_iterator itr = anim_map.begin();
    for(; itr != anim_map.end(); ++itr) {
      if(anim == itr->second) anim_name = itr->first;
      }
    if(anim_name.length() < 1) {
      anim = oldanim;
      return;
      }
    }
  else if(anim_map.count(anim_name)) {
    anim = mod[modnum]->LookUpAnimation(anim_name);
    }
  else {
    return;
    }

  if(which == 0) {
    anims[0] = anim;
    anims[1] = anim;
    times[0] = SDL_GetTicks();
    times[1] = SDL_GetTicks();
    }
  else if(which == 1) {
    anims[1] = anim;
    times[1] = SDL_GetTicks();
    }
  else {
    anims[0] = anim;
    times[0] = SDL_GetTicks();
    }
  if(anim_name.length() > 0) banner->SetText(anim_name);
  }

int main(int argc, char **argv) {
  int xs=1024, ys=1024;
  vector<string> modname;
  const char *weapname = "";
  Uint32 bgcolor = 0x000000;	// Default Background Color is Black

  int barg = 1;
  while(argc > barg && (strcmp(argv[barg], "-p") == 0
		|| strcmp(argv[barg], "-v") == 0
		|| strcmp(argv[barg], "-c") == 0
		|| strcmp(argv[barg], "-w") == 0
	)) {
    if(argc <= barg+1) {
      fprintf(stderr, "ERROR: -p/-w/-c requires file argument\n");
      exit(1);
      }
    if(strcmp(argv[barg], "-p") == 0) {
      SimpleModel::AddSourceFile(argv[barg+1]);
      }
    else if(strcmp(argv[barg], "-w") == 0) {
      weapname = argv[barg+1];
      }
    else if(strcmp(argv[barg], "-c") == 0) {
      int col = SimpleTexture::ColorByName(argv[barg+1]);
      bgcolor = SimpleTexture::BGColor(col)->r;
      bgcolor <<= 8;
      bgcolor += SimpleTexture::BGColor(col)->g;
      bgcolor <<= 8;
      bgcolor += SimpleTexture::BGColor(col)->b;
      }
    else if(strcmp(argv[barg], "-v") == 0) {
      verbose = true;
      barg += 1;
      continue;
      }
    barg += 2;
    }

  if((argc-barg) < 1) {
    fprintf(stderr, "USAGE: %s [-p <search_prefix> [...]] [-s <skin>] <model_file>|<model_dir> ...\n", argv[0]);
    exit(1);
    }

  if(!init_renderer(xs, ys, bgcolor)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }

  SimpleTexture::SetColor(0, 0.25, 0.25, 0.25, 0.75, 0.75, 0.75);
  banner = new SimpleTexture(0);
  banner->SetDefaultFontSize(50);
  banner->type = SIMPLETEXTURE_TRANSCOLOR;
  banner->SetTextAlignment(ST_ALIGN_CENTER);
  banner->SetText("Default Animation");
  banner->SetTextAspectRatio(16.0);

  vector<string> skinname;
  while(argc-barg >= 1) {
    if(argc-barg >= 2 && strcmp(argv[barg], "-s") == 0) {
      skinname.push_back(argv[barg + 1]);
      barg += 2;
      }
    else {
      fprintf(stderr, "Loading: '%s'\n", argv[barg]);
      mod.push_back(SM_LoadModel(argv[barg], skinname));
      if(!(mod.back())) {
        fprintf(stderr, "ERROR: Model load failed\n");
        exit(1);
        }
      if(strlen(weapname) > 0) {
        weap = SM_LoadModel(weapname);	//FIXME: Weapon Skin?
        if(weap) {
          mod.back()->AttachSubmodel("tag_weapon", weap);
          }
        }
      if(verbose) {
        map<string, int> anim_map = mod.back()->GetAnimations();
        map<string, int>::const_iterator anim = anim_map.begin();
        printf("Model has these animations:\n");
        for(; anim != anim_map.end(); ++anim) {
          printf("\t%s\n", anim->first.c_str());
          }
        }
      ++barg;
      }
    }

  anims.push_back(0);
  anims.push_back(0);
  times.push_back(SDL_GetTicks());
  times.push_back(SDL_GetTicks());

  { map<string, int> anim_map = mod.back()->GetAnimations();
    map<string, int>::const_iterator itr = anim_map.begin();
    for(; itr != anim_map.end(); ++itr) {
      if(0 == itr->second) banner->SetText(itr->first);
      }
    }

  int quit = 0;
  float yaw = 0.0, pitch = 0.0;
  float posx = 2.0, posy = 0.0, posz = 0.0;
  while(!quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
    quit = 1;
    }
      else if(event.type == SDL_KEYDOWN) {
	if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
	else if(event.key.keysym.sym == SDLK_RIGHTBRACKET) SetAnim(-1);
	else if(event.key.keysym.sym == SDLK_LEFTBRACKET) SetAnim(-2);

	else if(event.key.keysym.sym == SDLK_1) SetAnim(0, "BOTH_DEATH1");
	else if(event.key.keysym.sym == SDLK_2) SetAnim(0, "BOTH_DEAD1");
	else if(event.key.keysym.sym == SDLK_3) SetAnim(0, "BOTH_DEATH2");
	else if(event.key.keysym.sym == SDLK_4) SetAnim(0, "BOTH_DEAD2");
	else if(event.key.keysym.sym == SDLK_5) SetAnim(0, "BOTH_DEATH3");
	else if(event.key.keysym.sym == SDLK_6) SetAnim(0, "BOTH_DEAD3");

	else if(event.key.keysym.sym == SDLK_q) SetAnim(1, "TORSO_GESTURE");
	else if(event.key.keysym.sym == SDLK_w) SetAnim(1, "TORSO_ATTACK");
	else if(event.key.keysym.sym == SDLK_e) SetAnim(1, "TORSO_ATTACK2");
	else if(event.key.keysym.sym == SDLK_r) SetAnim(1, "TORSO_DROP");
	else if(event.key.keysym.sym == SDLK_t) SetAnim(1, "TORSO_RAISE");
	else if(event.key.keysym.sym == SDLK_y) SetAnim(1, "TORSO_STAND");
	else if(event.key.keysym.sym == SDLK_u) SetAnim(1, "TORSO_STAND2");

	else if(event.key.keysym.sym == SDLK_a) SetAnim(2, "LEGS_WALKCR");
	else if(event.key.keysym.sym == SDLK_s) SetAnim(2, "LEGS_WALK");
	else if(event.key.keysym.sym == SDLK_d) SetAnim(2, "LEGS_RUN");
	else if(event.key.keysym.sym == SDLK_f) SetAnim(2, "LEGS_BACK");
	else if(event.key.keysym.sym == SDLK_g) SetAnim(2, "LEGS_SWIM");
	else if(event.key.keysym.sym == SDLK_h) SetAnim(2, "LEGS_JUMP");
	else if(event.key.keysym.sym == SDLK_j) SetAnim(2, "LEGS_LAND");
	else if(event.key.keysym.sym == SDLK_k) SetAnim(2, "LEGS_JUMPB");
	else if(event.key.keysym.sym == SDLK_l) SetAnim(2, "LEGS_LANDB");
	else if(event.key.keysym.sym == SDLK_SEMICOLON) SetAnim(2, "LEGS_IDLE");
	else if(event.key.keysym.sym == SDLK_QUOTE) SetAnim(2, "LEGS_IDLECR");
	else if(event.key.keysym.sym == SDLK_RETURN) SetAnim(2, "LEGS_TURN");

	else if(event.key.keysym.sym == SDLK_RIGHT) yaw += 5.0;
	else if(event.key.keysym.sym == SDLK_LEFT)  yaw -= 5.0;
	else if(event.key.keysym.sym == SDLK_UP)   pitch += 5.0;
	else if(event.key.keysym.sym == SDLK_DOWN) pitch -= 5.0;

	else if(event.key.keysym.sym == SDLK_HOME) posy += 0.5;
	else if(event.key.keysym.sym == SDLK_END) posy -= 0.5;
	else if(event.key.keysym.sym == SDLK_PAGEDOWN) posz += 0.5;
	else if(event.key.keysym.sym == SDLK_PAGEUP) posz -= 0.5;
	else if(event.key.keysym.sym == SDLK_EQUALS)  posx += 0.5;
	else if(event.key.keysym.sym == SDLK_MINUS) posx -= 0.5;

	else if(event.key.keysym.sym == SDLK_KP6) posy += 0.5;
	else if(event.key.keysym.sym == SDLK_KP4) posy -= 0.5;
	else if(event.key.keysym.sym == SDLK_KP8) posz += 0.5;
	else if(event.key.keysym.sym == SDLK_KP2) posz -= 0.5;
	else if(event.key.keysym.sym == SDLK_KP_PLUS)  posx += 0.5;
	else if(event.key.keysym.sym == SDLK_KP_MINUS) posx -= 0.5;

	else if(event.key.keysym.sym == SDLK_SPACE) {
          if(modnum < mod.size() - 1) ++modnum;
          }
	else if(event.key.keysym.sym == SDLK_BACKSPACE) {
          if(modnum > 0) --modnum;
          }
	else {
	  }
	}
      }
    start_scene();
    glTranslatef(posx, posy, posz);
    glRotatef(yaw, 0.0, 0.0, 1.0);
    glRotatef(pitch, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, -2.0);
    mod[modnum]->Render(SDL_GetTicks(), anims, times);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, -11.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, banner->GLTexture());

    glBegin(GL_QUADS);
      glTexCoord2f(banner->ScaleX(0.0), banner->ScaleY(1.0));
      glVertex3f(0.0, 0.0, 0.0);
      glTexCoord2f(banner->ScaleX(1.0), banner->ScaleY(1.0));
      glVertex3f(1.0, 0.0, 0.0);
      glTexCoord2f(banner->ScaleX(1.0), banner->ScaleY(0.0));
      glVertex3f(1.0, 1.0, 0.0);
      glTexCoord2f(banner->ScaleX(0.0), banner->ScaleY(0.0));
      glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    finish_scene();
    }

  return 0;
  }
