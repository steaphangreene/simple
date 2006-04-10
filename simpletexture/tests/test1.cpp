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

#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

#include "../simpletexture.h"
#include "renderer.h"

int main(int argc, char **argv) {
  int xs = 1024, ys = 768;
  int offset = 1;

  if(argc < 2) {
    fprintf(stderr, "ERROR: requires at least one file argument\n");
    exit(1);
    }

  SimpleTexture *tex1 = new SimpleTexture(argv[1]);
  SimpleTexture *tex2 = new SimpleTexture(argv[1]);

  if(!init_renderer(xs, ys)) {
    fprintf(stderr, "Warning!  Graphics failed to init!\n");
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
        else if(event.key.keysym.sym == SDLK_q) quit = 1;
        else if(event.key.keysym.sym == SDLK_BACKSPACE) {
	  --offset;
	  if(offset < 1) offset = 1;
	  delete tex1;
	  delete tex2;
	  SimpleTexture::EmptyTrash();
	  tex1 = new SimpleTexture(argv[offset]);
	  tex2 = new SimpleTexture(argv[offset]);
	  }
        else if(event.key.keysym.sym == SDLK_SPACE) {
	  ++offset;
	  if(offset >= argc) offset = argc-1;
	  delete tex1;
	  delete tex2;
	  SimpleTexture::EmptyTrash();
	  tex1 = new SimpleTexture(argv[offset]);
	  tex2 = new SimpleTexture(argv[offset]);
	  }break;
        }
      }
    start_scene();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, tex1->GLTexture());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, tex1->yfact);
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(tex1->xfact, tex1->yfact);
    glVertex3f( 1.0, -1.0, 0.0);
    glTexCoord2f(tex1->xfact, 0.0);
    glVertex3f( 1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();

    glTranslatef(sin(SDL_GetTicks() / 1024.0) / 2.0,
	cos(SDL_GetTicks() / 1024.0) / 2.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, tex2->GLTexture());
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, tex2->yfact);
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(tex2->xfact, tex2->yfact);
    glVertex3f( 1.0, -1.0, 0.0);
    glTexCoord2f(tex2->xfact, 0.0);
    glVertex3f( 1.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    finish_scene();
    }

  delete tex1;
  delete tex2;
  SimpleTexture::EmptyTrash();

  return 0;
  }
