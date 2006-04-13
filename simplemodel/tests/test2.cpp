// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
#include "../simplemodel_mdx.h"
#include "renderer.h"

vector<int> anims;
vector<Uint32> times;

int main(int argc, char **argv) {
  int xs=800, ys=600;

  int barg = 1;
  while(argc > barg && (!strcmp(argv[barg], "-s"))) {
    if(argc <= barg+1) {
      fprintf(stderr, "ERROR: -s requires file argument\n");
      exit(1);
      }
    SimpleModel::AddSourceFile(argv[barg+1]);
    fprintf(stderr, "Added source %s\n", argv[barg+1]);
    barg += 2;
    }

  char *modname = "Bladedancer.mdx";

  if((argc-barg) > 2) {
    //fprintf(stderr, "USAGE: %s <modelfile>|<modeldir> [weapondir]\n", argv[0]$    fprintf(stderr, "USAGE: %s <modelfile>|<modeldir> [skin]\n", argv[0]);
    exit(1);
    }
  else if((argc-barg) >= 1) {
    modname = argv[barg];
    }

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
    }

  SimpleModel_MDX *mod = new SimpleModel_MDX("Bladedancer/Bladedancer", modname);
  if(!mod) {
    fprintf(stderr, "ERROR: Model load failed\n");
    exit(1);
    }

  anims.push_back(5);
  times.push_back(SDL_GetTicks());

  int quit = 0;
  float yaw = 0.0, pitch = 0.0;
  float posx = -200.0, posy = -30.0, posz = -60.0;
  while(!quit) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
    quit = 1;
    }
      else if(event.type == SDL_KEYDOWN) {
        if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
        else if(event.key.keysym.sym == SDLK_RIGHT) posx += 5.0;
        else if(event.key.keysym.sym == SDLK_LEFT)  posx -= 5.0;
        else if(event.key.keysym.sym == SDLK_UP)    posy += 5.0;
        else if(event.key.keysym.sym == SDLK_DOWN)  posy -= 5.0;
        else if(event.key.keysym.sym == SDLK_a)  posz += 5.0;
        else if(event.key.keysym.sym == SDLK_s)  posz -= 5.0;

        else {
          }
        }
      }
    start_scene();
    glTranslatef(posx, posy, posz);
    //glRotatef(yaw, 0.0, 0.0, 1.0);
    //glRotatef(pitch, 0.0, 1.0, 0.0);
    //glTranslatef(0.0, 0.0, -2.0);
    //glScalef(.00000005, .00000005, .00000005);
    mod->Render(SDL_GetTicks(), anims, times);
    finish_scene();
    }

  return 0;
  }
