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

#include <GL/gl.h>

#include "sg_globals.h"
#include "sg_colors.h"
#include "sg_panel.h"

SG_Panel::SG_Panel(SG_Texture bgtex) : SG_Widget() {
  texture.push_back(bgtex);
  state = 0;
  }

SG_Panel::~SG_Panel() {
  for(int tx = 0; tx < int(texture.size()); ++tx) {
    if(texture[tx].cur) {
      glFinish();
      SDL_FreeSurface(texture[tx].cur);
      texture[tx].cur = NULL;
      }
    if(texture[tx].texture) {
      glDeleteTextures(1, &(texture[tx].texture));
      texture[tx].texture = 0;
      }
    }
  }

bool SG_Panel::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Panel/Handle: Button Down at (%f,%f)\n", x, y);

  return 0;	// This widget eats all mouse events
  }

void SG_Panel::BuildTexture(int st) {
  if(texture[st].cur) {
    glFinish();
    SDL_FreeSurface(texture[st].cur);
    }
  if(texture[st].type == SG_TEXTURE_COLOR) {
    texture[st].cur = SDL_CreateRGBSurface(0, 16, 16, 32,
	SG_RGBA_COLFIELDS);
    SDL_FillRect(texture[st].cur, NULL, SDL_MapRGB(texture[st].cur->format,
	texture[st].col.r, texture[st].col.g, texture[st].col.b));
    }
  else if(texture[st].type == SG_TEXTURE_TRANS) {
    texture[st].cur = SDL_CreateRGBSurface(0, 16, 16, 32,
	SG_RGBA_COLFIELDS);
    }
  else if(texture[st].type == SG_TEXTURE_DEFINED) {
    texture[st].cur = SDL_CreateRGBSurface(0,
	texture[st].src->w, texture[st].src->h, 32,
	SG_RGBA_COLFIELDS);
    SDL_BlitSurface(texture[st].src, NULL, texture[st].cur, NULL);
    }

  if(!texture[st].texture) glGenTextures(1, &(texture[st].texture));
  glBindTexture(GL_TEXTURE_2D, texture[st].texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4,
		texture[st].cur->w, texture[st].cur->h, 0, GL_RGBA,
                GL_UNSIGNED_BYTE, texture[st].cur->pixels );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  texture[st].dirty = 0;
  }

bool SG_Panel::Render(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Panel %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  if(texture[state].dirty) BuildTexture(state);

  glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[state].texture);
  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, texture[state].yfact);
  glVertex3f(-1.0, -1.0, 0.0);
  glTexCoord2f(texture[state].xfact, texture[state].yfact);
  glVertex3f( 1.0, -1.0, 0.0);
  glTexCoord2f(texture[state].xfact, 0.0);
  glVertex3f( 1.0, 1.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, 1.0, 0.0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  return 1;
  }


//  bool SG_Panel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Panel::Default_Mouse_Cursor = NULL;
