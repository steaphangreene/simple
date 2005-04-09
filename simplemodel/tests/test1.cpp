// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_keysym.h"

#include <cstdio>
#include <string>

using namespace std;

#include "../simplemodel.h"
#include "../simplemodel_q3dir.h"
#include "../simplemodel_md3.h"
#include "../sm_q3anim.h"
#include "renderer.h"

vector<int> anims;
vector<Uint32> times;

static void SetAnim(int anim) {
  if(anim < BOTH_MAX) {
    anims[0] = anim;
    anims[1] = anim;
    times[0] = SDL_GetTicks();
    times[1] = SDL_GetTicks();
    }
  else if(anim < TORSO_MAX) {
    anims[1] = anim;
    times[1] = SDL_GetTicks();
    }
  else if(anim < LEGS_MAX) {
    anims[0] = anim;
    times[0] = SDL_GetTicks();
    }
  }

int main(int argc, char **argv) {
  int xs=640, ys=480;

  if(argc < 2 || argc > 3) {
    fprintf(stderr, "USAGE: %s <modelfile>|<modeldir> [weapondir]\n", argv[0]);
    exit(1);
    }

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }

  SimpleModel_MD3 *weap = NULL; //Created below
  SimpleModel *mod = SM_LoadModel(argv[1]);
  if(!mod) {
    fprintf(stderr, "ERROR: Model load failed\n");
    exit(1);
    }
  if(argc == 3) {
    weap = new SimpleModel_MD3(argv[2],
	(string(argv[2]) + "/machinegun.md3").c_str(),
	(string(argv[2]) + "/m41a.jpg").c_str());
    if(!weap) {
      fprintf(stderr, "ERROR: Weapon model load failed\n");
      exit(1);
      }
    ((SimpleModel_Q3Dir*)mod)->SetWeapon(weap);
    }

  anims.push_back(LEGS_IDLE);
  anims.push_back(TORSO_STAND);
  times.push_back(SDL_GetTicks());
  times.push_back(SDL_GetTicks());

  int quit = 0;
  while(!quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
	quit = 1;
	}
      else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
        else if(event.key.keysym.sym == SDLK_1) SetAnim(BOTH_DEATH1);
        else if(event.key.keysym.sym == SDLK_2) SetAnim(BOTH_DEAD1);
        else if(event.key.keysym.sym == SDLK_3) SetAnim(BOTH_DEATH2);
        else if(event.key.keysym.sym == SDLK_4) SetAnim(BOTH_DEAD2);
        else if(event.key.keysym.sym == SDLK_5) SetAnim(BOTH_DEATH3);
        else if(event.key.keysym.sym == SDLK_6) SetAnim(BOTH_DEAD3);

        else if(event.key.keysym.sym == SDLK_q) SetAnim(TORSO_GESTURE);
        else if(event.key.keysym.sym == SDLK_w) SetAnim(TORSO_ATTACK);
        else if(event.key.keysym.sym == SDLK_e) SetAnim(TORSO_ATTACK2);
        else if(event.key.keysym.sym == SDLK_r) SetAnim(TORSO_DROP);
        else if(event.key.keysym.sym == SDLK_t) SetAnim(TORSO_RAISE);
        else if(event.key.keysym.sym == SDLK_y) SetAnim(TORSO_STAND);
        else if(event.key.keysym.sym == SDLK_u) SetAnim(TORSO_STAND2);

        else if(event.key.keysym.sym == SDLK_a) SetAnim(LEGS_WALKCR);
        else if(event.key.keysym.sym == SDLK_s) SetAnim(LEGS_WALK);
        else if(event.key.keysym.sym == SDLK_d) SetAnim(LEGS_RUN);
        else if(event.key.keysym.sym == SDLK_f) SetAnim(LEGS_BACK);
        else if(event.key.keysym.sym == SDLK_g) SetAnim(LEGS_SWIM);
        else if(event.key.keysym.sym == SDLK_h) SetAnim(LEGS_JUMP);
        else if(event.key.keysym.sym == SDLK_j) SetAnim(LEGS_LAND);
        else if(event.key.keysym.sym == SDLK_k) SetAnim(LEGS_JUMPB);
        else if(event.key.keysym.sym == SDLK_l) SetAnim(LEGS_LANDB);
        else if(event.key.keysym.sym == SDLK_SEMICOLON) SetAnim(LEGS_IDLE);
        else if(event.key.keysym.sym == SDLK_QUOTE) SetAnim(LEGS_IDLECR);
        else if(event.key.keysym.sym == SDLK_RETURN) SetAnim(LEGS_TURN);
        else {
          }
        }
      }
    start_scene();
    mod->Render(SDL_GetTicks(), anims, times);
    finish_scene();
    }

  return 0;
  }
