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

#include <GL/gl.h>

#include "sg_panel.h"

void BuildTexture(SG_Texture &tex) {
  tex.cur = SDL_CreateRGBSurface(0, 16, 16, 32,
	0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_FillRect(tex.cur, NULL, SDL_MapRGB(tex.cur->format,
	tex.col.r, tex.col.g, tex.col.b));

  glGenTextures(1, &(tex.texture));
  glBindTexture(GL_TEXTURE_2D, tex.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_BGRA,
                GL_UNSIGNED_BYTE, tex.cur->pixels );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);
  }

SG_Panel::SG_Panel(float red, float green, float blue) : SG_Widget() {
  texture.resize(1);
  texture[0].type = SG_TEXTURE_COLOR;
  texture[0].col.r = (Uint8)(red*255.0f);
  texture[0].col.g = (Uint8)(green*255.0f);
  texture[0].col.b = (Uint8)(blue*255.0f);
  texture[0].src = NULL;
  texture[0].texture = 0;
  texture[0].xfact = 1.0;
  texture[0].yfact = 1.0;
  state = 0;

  BuildTexture(texture[0]);
  }

SG_Panel::~SG_Panel() {
  }

bool SG_Panel::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Panel/Handle: Button Down at (%f,%f)\n", x, y);

  return 0;	// This widget eats all mouse events
  }

bool SG_Panel::Render() {
//  fprintf(stderr, "Rendering Panel %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

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

