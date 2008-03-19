// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//  
// *************************************************************************

#include "SDL_opengl.h"

#include "sg_globals.h"
#include "sg_colors.h"
#include "sg_panel.h"

SG_Panel::SG_Panel(SimpleTexture tex) : SG_Widget() {
  texture.push_back(tex);
  state = 0;
  }

SG_Panel::~SG_Panel() {
// Deletion of SimpleTexture objects takes care of everything.
  }

int SG_Panel::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "Panel/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events

  if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }

  return 0;	// This widget eats all other mouse events all the time
  }

bool SG_Panel::RenderSelf(unsigned long cur_time) {
//  fprintf(stderr, "Rendering Panel %p!\n", this);

  glPushMatrix();

  if(texture[state].type == SIMPLETEXTURE_TRANS
	|| texture[state].type == SIMPLETEXTURE_TRANSCOLOR) {
    glEnable(GL_BLEND);
    }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[state].GLTexture());
  glColor3f(1.0f, 1.0f, 1.0f);


  glBegin(GL_QUADS);
  glTexCoord2f(texture[state].ScaleX(0.0), texture[state].ScaleY(1.0));
  glVertex3f(-1.0, -1.0, 0.0);
  glTexCoord2f(texture[state].ScaleX(1.0), texture[state].ScaleY(1.0));
  glVertex3f( 1.0, -1.0, 0.0);
  glTexCoord2f(texture[state].ScaleX(1.0), texture[state].ScaleY(0.0));
  glVertex3f( 1.0, 1.0, 0.0);
  glTexCoord2f(texture[state].ScaleX(0.0), texture[state].ScaleY(0.0));
  glVertex3f(-1.0, 1.0, 0.0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  if(texture[state].type == SIMPLETEXTURE_TRANS
	|| texture[state].type == SIMPLETEXTURE_TRANSCOLOR) {
    glDisable(GL_BLEND);
    }

  glPopMatrix();

  return true;
  }

void SG_Panel::SetTransparent(bool val) {
  for(int tx = 0; tx < int(texture.size()); ++tx) {
    if(texture[tx].type == SIMPLETEXTURE_COLOR
	|| texture[tx].type == SIMPLETEXTURE_TRANSCOLOR) {
      texture[tx].type = (val) ? SIMPLETEXTURE_TRANSCOLOR : SIMPLETEXTURE_COLOR;
      }
    else if(texture[tx].type == SIMPLETEXTURE_TRANS
	|| texture[tx].type == SIMPLETEXTURE_DEFINED) {
      texture[tx].type = (val) ? SIMPLETEXTURE_TRANS : SIMPLETEXTURE_DEFINED;
      }
    }
  }

//  bool SG_Panel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Panel::Default_Mouse_Cursor = NULL;

void SG_Panel::SetTexture(SimpleTexture tex, int st) {
  if(st < 0 || st >= (int)(texture.size())) return;
  texture[st].SetTexture(tex);
  }

void SG_Panel::SetAspectRatio(double asp) {
  aspect_ratio = asp;
  for(int tx = 0; tx < int(texture.size()); ++tx) {
    texture[tx].SetTextAspectRatio(asp);
    }
  }
