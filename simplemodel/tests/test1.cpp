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

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_keysym.h>
#include <cstdio>
#include <string>

using namespace std;

#include "../simplemodel.h"
#include "../sm_q3anim.h"
#include "renderer.h"

int main(int argc, char **argv) {
  int xs=640, ys=480;

  if(argc != 2) {
    fprintf(stderr, "USAGE: %s <modelfile>|<modeldir>\n", argv[0]);
    exit(1);
    }

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }

  SimpleModel *mod = SM_LoadModel(argv[1]);
  if(!mod) {
    fprintf(stderr, "ERROR: Model load failed\n");
    exit(1);
    }

  int quit = 0;
  while(!quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
	quit = 1;
	}
      else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
        else if(event.key.keysym.sym == SDLK_1)
				mod->SetAnimation(BOTH_DEATH1);
        else if(event.key.keysym.sym == SDLK_2)
				mod->SetAnimation(BOTH_DEAD1);
        else if(event.key.keysym.sym == SDLK_3)
				mod->SetAnimation(BOTH_DEATH2);
        else if(event.key.keysym.sym == SDLK_4)
				mod->SetAnimation(BOTH_DEAD2);
        else if(event.key.keysym.sym == SDLK_5)
				mod->SetAnimation(BOTH_DEATH3);
        else if(event.key.keysym.sym == SDLK_6)
				mod->SetAnimation(BOTH_DEAD3);

        else if(event.key.keysym.sym == SDLK_q)
				mod->SetAnimation(TORSO_GESTURE);
        else if(event.key.keysym.sym == SDLK_w)
				mod->SetAnimation(TORSO_ATTACK);
        else if(event.key.keysym.sym == SDLK_e)
				mod->SetAnimation(TORSO_ATTACK2);
        else if(event.key.keysym.sym == SDLK_r)
				mod->SetAnimation(TORSO_DROP);
        else if(event.key.keysym.sym == SDLK_t)
				mod->SetAnimation(TORSO_RAISE);
        else if(event.key.keysym.sym == SDLK_y)
				mod->SetAnimation(TORSO_STAND);
        else if(event.key.keysym.sym == SDLK_u)
				mod->SetAnimation(TORSO_STAND2);

        else if(event.key.keysym.sym == SDLK_a)
				mod->SetAnimation(LEGS_WALKCR);
        else if(event.key.keysym.sym == SDLK_s)
				mod->SetAnimation(LEGS_WALK);
        else if(event.key.keysym.sym == SDLK_d)
				mod->SetAnimation(LEGS_RUN);
        else if(event.key.keysym.sym == SDLK_f)
				mod->SetAnimation(LEGS_BACK);
        else if(event.key.keysym.sym == SDLK_g)
				mod->SetAnimation(LEGS_SWIM);
        else if(event.key.keysym.sym == SDLK_h)
				mod->SetAnimation(LEGS_JUMP);
        else if(event.key.keysym.sym == SDLK_j)
				mod->SetAnimation(LEGS_LAND);
        else if(event.key.keysym.sym == SDLK_k)
				mod->SetAnimation(LEGS_JUMPB);
        else if(event.key.keysym.sym == SDLK_l)
				mod->SetAnimation(LEGS_LANDB);
        else if(event.key.keysym.sym == SDLK_SEMICOLON)
				mod->SetAnimation(LEGS_IDLE);
        else if(event.key.keysym.sym == SDLK_QUOTE)
				mod->SetAnimation(LEGS_IDLECR);
        else if(event.key.keysym.sym == SDLK_RETURN)
				mod->SetAnimation(LEGS_TURN);
        else {
          }
        }
      }
    start_scene();
    mod->Render(SDL_GetTicks());
    finish_scene();
    }

  return 0;
  }
