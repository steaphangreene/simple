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

#include "SDL_opengl.h"

#include "SDL_ttf.h"

#include "sg_textarea.h"
#include "sg_globals.h"

#include <math.h>

SG_TextArea::SG_TextArea(string mes, SG_Texture tex, SG_Texture dis_tex,
	float mx, float my) : SG_Panel(tex) {
  texture.push_back(dis_tex);
  rendered_text = NULL;
  text_width = 0;
  visible_lines = 1;
  xoffset = 0.0;
  yoffset = 0.0;
  scroll_ystart = 0.0;
  scroll_yend = 0.0;
  scroll_time = 0;
  xmargin = mx;
  ymargin = my;
  font_size = -1; //AUTO
  SetText(mes);
  }

SG_TextArea::~SG_TextArea() {
  if(rendered_text != NULL) SDL_FreeSurface(rendered_text);
  rendered_text = NULL;
  }

int SG_TextArea::HandleEvent(SDL_Event *event, float x, float y) {
//  if(event->type == SDL_MOUSEBUTTONDOWN)
//    fprintf(stderr, "TextArea/Handle: Button Down at (%f,%f)\n", x, y);

  if(flags & SG_WIDGET_FLAGS_IGNORE) return -1; //Ignore all events

  return 0;	//This widget eats all events all the time
  }

//  bool SG_TextArea::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_TextArea::Default_Mouse_Cursor = NULL;

void SG_TextArea::SetMargins(float xmar, float ymar) {
  xmargin = xmar;
  ymargin = ymar;
  }

void SG_TextArea::SetText(const string &mes) {
  message = mes;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  if(rendered_text != NULL) SDL_FreeSurface(rendered_text);
  rendered_text = NULL;
  UpdateLines();
  }

void SG_TextArea::SetFontSize(int sz) {
  font_size = sz;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  if(rendered_text != NULL) SDL_FreeSurface(rendered_text);
  rendered_text = NULL;
  UpdateLines();
  }

const string &SG_TextArea::Text() {
  return message;
  }

int nextpoweroftwo(int x) {
  if(x <= 2) return 2;

  --x;          //Hitch it down in case it's exactly a power of 2
  int p = 1;
  for(; x != 1; ++p, x>>=1);
  x <<= p;
  return x;
  }

void SG_TextArea::BuildTexture(int st) {
  if(message.length() < 1) {
    SG_Panel::BuildTexture(st);
    return;
    }
  if(current_sg == NULL || current_sg->Font() == NULL) {
    fprintf(stderr, "WARNING: No Font Loaded!\n");
    SG_Panel::BuildTexture(st);
    return;
    }

  int xsize = 0, ysize = 0, xoff = 0, yoff = 0;

  if(texture[st].type == SG_TEXTURE_DEFINED) {
    int bxsize = texture[st].src->w;
    int bysize = texture[st].src->h;

			//Used temporarilly - not final values
    xsize = int((float)(bxsize) * (1.0f - xmargin * 2.0f) + 0.5f);
    ysize = int((float)(bysize) * (1.0f - ymargin * 2.0f) + 0.5f);

    font_size = ysize / visible_lines;
    if(current_sg->Font(font_size) == NULL) {
      fprintf(stderr, "WARNING: Couldn't resize font to ptsize %d\n", font_size);
      SG_Panel::BuildTexture(st);
      return;
      }

    xoff = (bxsize-xsize)/2;
    yoff = (bysize-ysize)/2;

    //OpenGL Needs a power of two size - grow to next
    xsize = nextpoweroftwo(bxsize);	// Final values
    ysize = nextpoweroftwo(bysize);

    texture[st].xfact = (float)(bxsize) / (float)(xsize);
    texture[st].yfact = (float)(bysize) / (float)(ysize);
    }
  else {
    int bxsize = text_width;
    int bysize = TTF_FontHeight(current_sg->Font(font_size)) * lines.size();

    xsize = bxsize;	//Used temporarilly - not final values
    ysize = bysize;
    bxsize = int((float)(bxsize) / (1.0f - xmargin * 2.0f) + 0.5f);
    bysize = int((float)(bysize) / (1.0f - ymargin * 2.0f) + 0.5f);
    xoff = (bxsize-xsize)/2;
    yoff = (bysize-ysize)/2;

    //OpenGL Needs a power of two size - grow to next
    xsize = nextpoweroftwo(bxsize);	// Final values
    ysize = nextpoweroftwo(bysize);

    int max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);

    texture[st].xfact = (float)(bxsize) / (float)(xsize);
    texture[st].yfact = (float)(bysize) / (float)(ysize);
    }

  if(texture[st].cur) SDL_FreeSurface(texture[st].cur);
  texture[st].cur = NULL;

  if(texture[st].type == SG_TEXTURE_COLOR) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32,
	SG_SDL_RGBA_COLFIELDS);
    SDL_FillRect(texture[st].cur, NULL, SDL_MapRGB(texture[st].cur->format,
	texture[st].col.r, texture[st].col.g, texture[st].col.b));
    }
  else if(texture[st].type == SG_TEXTURE_TRANSCOLOR) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	SG_SDL_RGBA_COLFIELDS);
    }
  else if(texture[st].type == SG_TEXTURE_DEFINED) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	SG_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(texture[st].src, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(texture[st].src, NULL, texture[st].cur, NULL);
    }
  else if(texture[st].type == SG_TEXTURE_TRANS) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	SG_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(texture[st].src, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(texture[st].src, NULL, texture[st].cur, NULL);
    }

  SDL_Rect srec = { 0, 0, 0, 0}, drec = { xoff, yoff, 0, 0 };
  for(int ln = 0; ln < int(lines.size()); ++ln) {
    if(lines[ln].length() > 0) {
      rendered_text =
	TTF_RenderText_Blended(current_sg->Font(font_size), lines[ln].c_str(),
		texture[st].fg);
      if(!rendered_text) {
	fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
	exit(1);
	}
      srec.w = rendered_text->w;
      srec.h = rendered_text->h;
      if(texture[st].type == SG_TEXTURE_TRANS
		|| texture[st].type == SG_TEXTURE_TRANSCOLOR) {
	SDL_SetAlpha(rendered_text, 0, SDL_ALPHA_TRANSPARENT);
	}
      SDL_BlitSurface(rendered_text, &srec, texture[st].cur, &drec);
      SDL_FreeSurface(rendered_text);
      rendered_text = NULL;
      }
    drec.y += TTF_FontHeight(current_sg->Font(font_size));
    }

  texture[st].Update();
  }

void SG_TextArea::Disable() {
  flags |= SG_WIDGET_FLAGS_DISABLED;
  state = 1;
  }
 
void SG_TextArea::Enable() {
  flags &= (~SG_WIDGET_FLAGS_DISABLED);
  state = 0;
  }

void SG_TextArea::SetVisibleLines(int numlns) {
  visible_lines = numlns;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  }

void SG_TextArea::SetXOffset(float xoff) {
  xoffset = xoff;
  }

void SG_TextArea::SetYOffset(float yoff) {
  yoffset = yoff;
  }

void SG_TextArea::SetOffsets(float xoff, float yoff) {
  SetXOffset(xoff);
  SetYOffset(yoff);
  }

void SG_TextArea::AutoScroll(float startyoff, float endyoff, Uint32 starttime, int loop) {
  scroll_loop = loop;
  scroll_time = starttime;
  scroll_ystart = startyoff;
  scroll_yend = endyoff;
  }

void SG_TextArea::StopScroll() {
  scroll_ystart = 0.0;
  scroll_yend = 0.0;
  scroll_time = 0;
  }

void SG_TextArea::UpdateLines() {
  int pos = 0, lpos=0;
  while(lpos < (int)(message.length())) {
    pos = message.find('\n', lpos);
    if(pos < lpos) pos = message.length();
    lines.push_back(message.substr(lpos, pos - lpos));
    int xs = 0, ys = 0;
    TTF_SizeText(current_sg->Font(font_size), lines.back().c_str(), &xs, &ys);
    if(text_width < xs) text_width = xs;
    lpos = pos+1;
    }
  }
