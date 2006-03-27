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

SG_TextArea::SG_TextArea(string mes, SimpleTexture tex, SimpleTexture dis_tex,
	float mx, float my) : SG_Panel(tex) {
  texture.push_back(dis_tex);
  rendered_text = NULL;
  text_xsize = 0;
  text_ysize = 0;
  visible_xlines = SG_AUTOSIZE;
  visible_ylines = SG_AUTOSIZE;
  xmargin = mx;
  ymargin = my;
  font_size = SG_AUTOSIZE; //AUTO
  alignment = SG_ALIGN_LEFT;
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

  if(event->type == SDL_MOUSEBUTTONDOWN
	&& (event->button.button == 4 || event->button.button == 5)) {
		// Allow mousewheel events to pass through
    return -1;
    }

  return 0;	//This widget eats all other events all the time
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
  lines.clear();
  UpdateLines();
  UpdateRange();
  }

void SG_TextArea::SetFontSize(int sz) {
  font_size = sz;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  if(rendered_text != NULL) SDL_FreeSurface(rendered_text);
  rendered_text = NULL;
  lines.clear();	//FIXME: Only really need to redetect XSize
  UpdateLines();
  UpdateRange();
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

  int bxsize = 0, bysize = 0, xsize = 0, ysize = 0, xoff = 0, yoff = 0;

  if(texture[st].type == SIMPLETEXTURE_DEFINED) {
    bxsize = texture[st].src->w;
    bysize = texture[st].src->h;

			//Used temporarilly - not final values
    xsize = int((float)(bxsize) * (1.0f - xmargin * 2.0f) + 0.5f);
    ysize = int((float)(bysize) * (1.0f - ymargin * 2.0f) + 0.5f);

    if(font_size != (int)((double)(ysize) / YSpan() + 0.5)) {
      SetFontSize((int)((double)(ysize) / YSpan() + 0.5));
      }

    if(text_xsize < xsize) {
      text_xsize = xsize;  //To be sure alignment works!
      UpdateRange();
      }

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
    double fsz = (double)(TTF_FontHeight(current_sg->Font(font_size)));
    bxsize = (int)(fsz * XSpan() + 0.5);
    bysize = (int)(fsz * YSpan() + 0.5);

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

  if(texture[st].type == SIMPLETEXTURE_COLOR) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32,
	ST_SDL_RGBA_COLFIELDS);
    SDL_FillRect(texture[st].cur, NULL, SDL_MapRGB(texture[st].cur->format,
	texture[st].col.r, texture[st].col.g, texture[st].col.b));
    }
  else if(texture[st].type == SIMPLETEXTURE_TRANSCOLOR) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	ST_SDL_RGBA_COLFIELDS);
    }
  else if(texture[st].type == SIMPLETEXTURE_DEFINED) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	ST_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(texture[st].src, 0, SDL_ALPHA_OPAQUE);
    SDL_BlitSurface(texture[st].src, NULL, texture[st].cur, NULL);
    }
  else if(texture[st].type == SIMPLETEXTURE_TRANS) {
    texture[st].cur = SDL_CreateRGBSurface(0, xsize, ysize, 32, 
	ST_SDL_RGBA_COLFIELDS);
    SDL_SetAlpha(texture[st].src, 0, SDL_ALPHA_TRANSPARENT);
    SDL_BlitSurface(texture[st].src, NULL, texture[st].cur, NULL);
    }

  if(rendered_text == NULL) {	// Create persistent text surface
    SDL_Rect drec = { 0, 0, 0, 0 };
    rendered_text = SDL_CreateRGBSurface(0, text_xsize, text_ysize, 32,
	ST_SDL_RGBA_COLFIELDS);
    for(int ln = 0; ln < int(lines.size()); ++ln) {
      if(lines[ln].length() > 0) {
	SDL_Surface *tmp_text = TTF_RenderText_Blended(
		current_sg->Font(font_size), lines[ln].c_str(), texture[st].fg);
	if(!tmp_text) {
	  fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
	  exit(1);
	  }

	if(alignment == SG_ALIGN_RIGHT) {
	  int xs = 0, ys = 0;
	  TTF_SizeText(current_sg->Font(font_size), lines[ln].c_str(), &xs, &ys);
	  drec.x = text_xsize - xs;
	  }
	else if(alignment == SG_ALIGN_CENTER) {
	  int xs = 0, ys = 0;
	  TTF_SizeText(current_sg->Font(font_size), lines[ln].c_str(), &xs, &ys);
	  drec.x = (text_xsize - xs)/2;
	  }

	SDL_SetAlpha(tmp_text, 0, SDL_ALPHA_TRANSPARENT);
	SDL_BlitSurface(tmp_text, NULL, rendered_text, &drec);
	SDL_FreeSurface(tmp_text);
	}
      drec.y += TTF_FontHeight(current_sg->Font(font_size));
      }
    }

  { SDL_Rect srec = { 0, 0, 0, 0}, drec = { xoff, yoff, 0, 0 };
    double fsz = (double)(TTF_FontHeight(current_sg->Font(font_size)));
    srec.x = (int)(fsz * XValue() + 0.5);
    srec.w = (int)(fsz * XSpan() + 0.5);

    if(YValue() >= 0.0) {
      srec.y = (int)(fsz * YValue() + 0.5);
      srec.h = (int)(fsz * YSpan() + 0.5);
      }
    else {
      drec.y += (int)(fsz * -YValue() + 0.5);
      srec.h = (int)(fsz * (YSpan() + YSpan()) + 0.5);
      }

    if(texture[st].type == SIMPLETEXTURE_TRANS
		|| texture[st].type == SIMPLETEXTURE_TRANSCOLOR) {
      SDL_SetAlpha(rendered_text, 0, SDL_ALPHA_TRANSPARENT);
      }
    SDL_BlitSurface(rendered_text, &srec, texture[st].cur, &drec);
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

void SG_TextArea::SetVisibleSize(double xs, double ys) {
  visible_xlines = xs;
  visible_ylines = ys;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  UpdateLines();
  UpdateRange();
  }

void SG_TextArea::SetVisibleLines(int numlns) {	//Depricated!
  visible_ylines = numlns;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  UpdateLines();
  UpdateRange();
  }

void SG_TextArea::UpdateLines() {
  if(lines.size() == 0) {
    text_xsize = 0;
    int pos = 0, lpos = 0;
    while(lpos < (int)(message.length())) {
      pos = message.find('\n', lpos);
      if(pos < lpos) pos = message.length();
      lines.push_back(message.substr(lpos, pos - lpos));
      int xs = 0, ys = 0;
      TTF_SizeText(current_sg->Font(font_size), lines.back().c_str(), &xs, &ys);
      if(text_xsize < xs) text_xsize = xs;
      lpos = pos+1;
      }
    text_ysize = TTF_FontHeight(current_sg->Font(font_size)) * lines.size();
    }
  }

void SG_TextArea::UpdateRange() {
  double font_height = TTF_FontHeight(current_sg->Font(font_size));
  if(visible_xlines > 0) {
    SetXLimits(0.0, (double)(text_xsize) / font_height);
    SetXSpan((double)(visible_xlines));
    }
  else {
    SetXLimits(0.0, (double)(text_xsize) / font_height);
    SetXSpan((double)(text_xsize) / font_height);
    }

  if(visible_ylines > 0) {	//Allows scrolling before/after text vertically
    SetYLimits(-(double)(visible_ylines), (double)(lines.size()) + (double)(visible_ylines));
    SetYSpan((double)(visible_ylines));
    }
  else {
    SetYLimits(-(double)(lines.size()), (double)(lines.size()) * 2.0);
    SetYSpan((double)(lines.size()));
    }
  }

void SG_TextArea::RangerChanged() {
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  }

void SG_TextArea::SetAlignment(int align) {
  alignment = align;
  for(int tx=0; tx < int(texture.size()); ++tx) texture[tx].dirty = 1;
  if(rendered_text != NULL) SDL_FreeSurface(rendered_text);
  rendered_text = NULL;
  }
