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
        else if(event.key.keysym.sym == SDLK_1) mod->SetAnimation(0);
        else if(event.key.keysym.sym == SDLK_2) mod->SetAnimation(1);
        else if(event.key.keysym.sym == SDLK_3) mod->SetAnimation(2);
        else if(event.key.keysym.sym == SDLK_4) mod->SetAnimation(3);
        else if(event.key.keysym.sym == SDLK_5) mod->SetAnimation(4);
        else if(event.key.keysym.sym == SDLK_6) mod->SetAnimation(5);
        else if(event.key.keysym.sym == SDLK_q) mod->SetAnimation(6);
        else if(event.key.keysym.sym == SDLK_w) mod->SetAnimation(7);
        else if(event.key.keysym.sym == SDLK_e) mod->SetAnimation(8);
        else if(event.key.keysym.sym == SDLK_r) mod->SetAnimation(9);
        else if(event.key.keysym.sym == SDLK_t) mod->SetAnimation(10);
        else if(event.key.keysym.sym == SDLK_y) mod->SetAnimation(11);
        else if(event.key.keysym.sym == SDLK_u) mod->SetAnimation(12);
        else if(event.key.keysym.sym == SDLK_i) mod->SetAnimation(13);
        else if(event.key.keysym.sym == SDLK_o) mod->SetAnimation(14);
        else if(event.key.keysym.sym == SDLK_p) mod->SetAnimation(15);
        else if(event.key.keysym.sym == SDLK_a) mod->SetAnimation(16);
        else if(event.key.keysym.sym == SDLK_s) mod->SetAnimation(17);
        else if(event.key.keysym.sym == SDLK_d) mod->SetAnimation(18);
        else if(event.key.keysym.sym == SDLK_f) mod->SetAnimation(19);
        else if(event.key.keysym.sym == SDLK_g) mod->SetAnimation(20);
        else if(event.key.keysym.sym == SDLK_h) mod->SetAnimation(21);
        else if(event.key.keysym.sym == SDLK_j) mod->SetAnimation(22);
        else if(event.key.keysym.sym == SDLK_k) mod->SetAnimation(23);
        else if(event.key.keysym.sym == SDLK_l) mod->SetAnimation(24);
        else if(event.key.keysym.sym == SDLK_SEMICOLON) mod->SetAnimation(25);
        else if(event.key.keysym.sym == SDLK_QUOTE) mod->SetAnimation(26);
        else if(event.key.keysym.sym == SDLK_RETURN) mod->SetAnimation(27);
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
