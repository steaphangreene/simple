// *************************************************************************
//  This file is part of the Simple* Example Suite by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  Simple* is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  Simple* is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Simple* (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_keysym.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

#include "../simpletexture.h"
#include "simplevideo.h"

int main(int argc, char **argv) {
  int xs = 1024, ys = 768;
  int offset = 1;

  if(argc < 2) {
    fprintf(stderr, "ERROR: requires at least one file argument\n");
    exit(1);
    }

  SimpleTexture *tex = new SimpleTexture(argv[1]);

  // Set up SimpleVideo, aligned just like old renderer was
  SimpleVideo *video = new SimpleVideo(xs, ys, 0.0);
  video->SetDown(0.0, 0);
  video->SetAngle(90.0, 0);
  video->SetPosition(6.0, 0.0, 0);

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
	  delete tex;
	  SimpleTexture::EmptyTrash();
	  tex = new SimpleTexture(argv[offset]);
	  }
        else if(event.key.keysym.sym == SDLK_SPACE) {
	  ++offset;
	  if(offset >= argc) offset = argc-1;
	  delete tex;
	  SimpleTexture::EmptyTrash();
	  tex = new SimpleTexture(argv[offset]);
	  }break;
        }
      }
    video->StartScene();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, tex->GLTexture());
    glBegin(GL_QUADS);
    glTexCoord2f(tex->ScaleX(0.0), tex->ScaleY(1.0));
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(tex->ScaleX(1.0), tex->ScaleY(1.0));
    glVertex3f( 1.0, -1.0, 0.0);
    glTexCoord2f(tex->ScaleX(1.0), tex->ScaleY(0.0));
    glVertex3f( 1.0, 1.0, 0.0);
    glTexCoord2f(tex->ScaleX(0.0), tex->ScaleY(0.0));
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();

    glTranslatef(sin(SDL_GetTicks() / 1024.0) / 2.0,
	cos(SDL_GetTicks() / 1024.0) / 2.0, 0.0);
    glBindTexture(GL_TEXTURE_2D, tex->GLTexture());
    glBegin(GL_QUADS);
    glTexCoord2f(tex->ScaleX(0.0), tex->ScaleY(1.0));
    glVertex3f(-1.0, -1.0, 0.0);
    glTexCoord2f(tex->ScaleX(1.0), tex->ScaleY(1.0));
    glVertex3f( 1.0, -1.0, 0.0);
    glTexCoord2f(tex->ScaleX(1.0), tex->ScaleY(0.0));
    glVertex3f( 1.0, 1.0, 0.0);
    glTexCoord2f(tex->ScaleX(0.0), tex->ScaleY(0.0));
    glVertex3f(-1.0, 1.0, 0.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    video->FinishScene();

    SDL_Delay(10);	// Let the OS breathe
    }

  delete tex;
  SimpleTexture::EmptyTrash();

  return 0;
  }
