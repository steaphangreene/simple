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

#include "sg_translabel.h"

#include <SDL/SDL_ttf.h>
extern TTF_Font *font;

SG_TransLabel::SG_TransLabel(string mes, float red, float green, float blue)
	: SG_Widget() {
  message = mes;
  fg.r = (Uint8)(red*255.0f);
  fg.g = (Uint8)(green*255.0f);
  fg.b = (Uint8)(blue*255.0f);

  char *fontfn = "fonts/Adventure Subtitles Normal.ttf";
  if(!font) {
    if(TTF_Init()) {
      fprintf(stderr, "ERROR: Unable to load font '%s' - %s\n", TTF_GetError());      exit(1);
      }
    atexit(TTF_Quit);

    font = TTF_OpenFont(fontfn, 100);
    if(!font) {
      fprintf(stderr, "ERROR: Unable to load font '%s'!\n", fontfn);
      exit(1);
      }
    }

  BuildTransTexture();
  }

SG_TransLabel::~SG_TransLabel() {
  if(font) TTF_CloseFont(font);
  font = NULL;

  glFinish();   //Be sure we don't pull the rug out from under GL
  SDL_FreeSurface(cur);
  glDeleteTextures(1, &texture);
  }

bool SG_TransLabel::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "TransLabel/Handle: Button Down at (%f,%f)\n", x, y);

  return 0;	// This widget eats all mouse events
  }

bool SG_TransLabel::Render() {
//  fprintf(stderr, "Rendering TransLabel %p!\n", this);

  if(flags & SG_WIDGET_FLAGS_HIDDEN) return 1;

  glPushMatrix();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glColor3f(1.0f, 1.0f, 1.0f);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, yfact);
  glVertex3f(-1.0, -1.0, 0.0);
  glTexCoord2f(xfact, yfact);
  glVertex3f( 1.0, -1.0, 0.0);
  glTexCoord2f(xfact, 0.0);
  glVertex3f( 1.0, 1.0, 0.0);
  glTexCoord2f(0.0, 0.0);
  glVertex3f(-1.0, 1.0, 0.0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  return 1;
  }


extern int nextpoweroftwo(int x);

void SG_TransLabel::BuildTransTexture() {
  if(message.length() < 1) {
    texture = 0;
    return;
    }

  SDL_Surface *tmp_text;
  int xsize, ysize;

  tmp_text = TTF_RenderText_Blended(font, message.c_str(), fg);
  if(!tmp_text) {
    fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
    exit(1);
    }

  //OpenGL Needs a power of two size - grow to next
  xsize = nextpoweroftwo(tmp_text->w);
  ysize = nextpoweroftwo(tmp_text->h);

  cur = SDL_CreateRGBSurface(0, xsize, ysize, 32,
                        0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
//  SDL_FillRect(cur, NULL, SDL_MapRGB(cur->format,
//        col.r, col.g, col.b));

  SDL_BlitSurface(tmp_text, 0, cur, 0);

  glGenTextures(1, &(texture));
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_BGRA,
                GL_UNSIGNED_BYTE, cur->pixels );

  // Setup Texture Params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glBindTexture(GL_TEXTURE_2D, 0);

  xfact = (float)(tmp_text->w) / (float)(xsize);
  yfact = (float)(tmp_text->h) / (float)(ysize);

  SDL_FreeSurface(tmp_text);

// FIXME - implement new version of this.
//  glTranslatef(0.0, 0.0, 0.0625);             //Advance to next "layer"
//  glScalef(1.0-xmargin, 1.0-ymargin, 1.0);    //Scale for margins
  }


//  bool SG_TransLabel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_TransLabel::Default_Mouse_Cursor = NULL;

void SG_TransLabel::SetMargins(float xmar, float ymar) {
  xmargin = xmar;
  ymargin = ymar;
  }
