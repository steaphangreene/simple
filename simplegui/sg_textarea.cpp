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

#include <SDL/SDL_ttf.h>

#include "sg_textarea.h"
#include "sg_globals.h"

#include <math.h>

int nextpoweroftwo(int x)
{
        double logbase2 = log(x) / log(2);
        return int(round(pow(2,ceil(logbase2))));
}


void BuildTexture(SG_Texture &tex, string mes, float mx, float my) {
  if(mes.length() < 1) {
    BuildTexture(tex);
    return;
    }
  if(current_sg == NULL || current_sg->Font() == NULL) {
    fprintf(stderr, "WARNING: No Font Loaded!\n");
    BuildTexture(tex);
    return;
    }

  SDL_Surface *tmp_text;
  int xsize, ysize;

  tmp_text = TTF_RenderText_Blended(current_sg->Font(), mes.c_str(), tex.fg);
  if(!tmp_text) {
    fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
    exit(1);
    }

  //OpenGL Needs a power of two size - grow to next
  xsize = nextpoweroftwo(tmp_text->w);
  ysize = nextpoweroftwo(tmp_text->h);

  tex.cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_FillRect(tex.cur, NULL, SDL_MapRGB(tex.cur->format,
	tex.col.r, tex.col.g, tex.col.b));

  SDL_BlitSurface(tmp_text, 0, tex.cur, 0);

  glGenTextures(1, &(tex.texture));
  glBindTexture(GL_TEXTURE_2D, tex.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_BGRA, 
		GL_UNSIGNED_BYTE, tex.cur->pixels );

  // Setup Texture Params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       

  glBindTexture(GL_TEXTURE_2D, 0);

  tex.xfact = (float)(tmp_text->w) / (float)(xsize);
  tex.yfact = (float)(tmp_text->h) / (float)(ysize);

  SDL_FreeSurface(tmp_text);

// FIXME - implement new version of this.
//  glTranslatef(0.0, 0.0, 0.0625);		//Advance to next "layer"
//  glScalef(1.0-xmargin, 1.0-ymargin, 1.0);	//Scale for margins
  }

SG_TextArea::SG_TextArea(string mes,
	float red, float green, float blue,
	float tred, float tgreen, float tblue)
		: SG_Panel(red, green, blue) {
  texture[0].fg.r = (Uint8)(tred*255.0f);
  texture[0].fg.g = (Uint8)(tgreen*255.0f);
  texture[0].fg.b = (Uint8)(tblue*255.0f);
  message = mes;
  xmargin = 0.0;
  ymargin = 0.0;

  BuildTexture(texture[0], message, xmargin, ymargin);
  }

SG_TextArea::~SG_TextArea() {
  glFinish();	//Be sure we don't pull the rug out from under GL
  SDL_FreeSurface(texture[0].cur);
  glDeleteTextures(1, &(texture[0].texture));
  }

bool SG_TextArea::HandleMouseEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "TextArea/Handle: Button Down at (%f,%f)\n", x, y);
  return 0;	//This widget eats all events
  }

//  bool SG_TextArea::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_TextArea::Default_Mouse_Cursor = NULL;

void SG_TextArea::SetMargins(float xmar, float ymar) {
  xmargin = xmar;
  ymargin = ymar;
  }

