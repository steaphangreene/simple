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

#include <SDL/SDL_opengl.h>

#include <SDL/SDL_ttf.h>

#include "sg_textarea.h"
#include "sg_globals.h"

#include <math.h>

SG_TextArea::SG_TextArea(string mes, SG_Texture tex, SG_Texture dis_tex,
	float mx, float my) : SG_Panel(tex) {
  texture.push_back(dis_tex);
  lines_visible = 1;
  message = mes;
  xmargin = mx;
  ymargin = my;
  }

SG_TextArea::~SG_TextArea() {
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
  }

const string &SG_TextArea::Text() {
  return message;
  }

int nextpoweroftwo(int x)
{
        double logbase2 = log(x) / log(2);
        return int(round(pow(2,ceil(logbase2))));
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

  SDL_Surface *tmp_text = NULL;
  int xsize = 0, ysize = 0, xoff = 0, yoff = 0, fontsz = -1;

  vector<string> line;
  int maxx = 80, maxy = 40;

  { int pos = 0, lpos=0;
    while(lpos < int(message.length()) && int(line.size()) < maxy) {
      pos = message.find('\n', lpos);
      if(pos <= lpos) pos = message.length();
      line.push_back(message.substr(lpos, pos - lpos));
      lpos = pos+1;
      }
    }

  if(texture[st].type == SG_TEXTURE_DEFINED) {
    int bxsize = texture[st].src->w;
    int bysize = texture[st].src->h;

			//Used temporarilly - not final values
    xsize = int((float)(bxsize) * (1.0f - xmargin * 2.0f) + 0.5f);
    ysize = int((float)(bysize) * (1.0f - ymargin * 2.0f) + 0.5f);

    fontsz = ysize / lines_visible;
    if(current_sg->Font(fontsz) == NULL) {
      fprintf(stderr, "WARNING: Couldn't resize font to ptsize %d\n", fontsz);
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
  else while(1) { 
    int bxsize = 0, bysize = 0;
    int pos = 0, lpos=0, tmpx=0, tmpy=0;
						//FIXME: Scroll?
    line.clear();
    while(lpos < int(message.length()) && int(line.size()) < maxy) {
      pos = message.find('\n', lpos);
      if(pos < lpos) pos = message.length();
      line.push_back(message.substr(lpos, pos - lpos));

      //FIXME: Scroll?
      if(int(line.back().length()) > maxx)
	line.back() = line.back().substr(0, maxx);

      lpos = pos+1;
      TTF_SizeText(current_sg->Font(fontsz), line.back().c_str(), &tmpx, &tmpy);
      if(bxsize < tmpx) bxsize = tmpx;
      bysize += tmpy;
      }

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

    if(xsize > max && ysize > max) { --maxx; --maxy; }
    else if(xsize > max) { --maxx; }
    else if(ysize > max) { --maxy; }
    else break;
    }

  if(texture[st].cur) SDL_FreeSurface(texture[st].cur);
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
  for(int ln = 0; ln < int(line.size()); ++ln) {
    if(line[ln].length() > 0) {
      tmp_text =
	TTF_RenderText_Blended(current_sg->Font(fontsz), line[ln].c_str(),
		texture[st].fg);
      if(!tmp_text) {
	fprintf(stderr, "ERROR: Failed to render font: %s\n", TTF_GetError());
	exit(1);
	}
      srec.w = tmp_text->w;
      srec.h = tmp_text->h;
      if(texture[st].type == SG_TEXTURE_TRANS
		|| texture[st].type == SG_TEXTURE_TRANSCOLOR) {
	SDL_SetAlpha(tmp_text, 0, SDL_ALPHA_TRANSPARENT);
	}
      SDL_BlitSurface(tmp_text, &srec, texture[st].cur, &drec);
      SDL_FreeSurface(tmp_text);
      }
    drec.y += TTF_FontHeight(current_sg->Font(fontsz));
    }

  if(!texture[st].texture) glGenTextures(1, &(texture[st].texture));
  glBindTexture(GL_TEXTURE_2D, texture[st].texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, xsize, ysize, 0, GL_RGBA, 
		GL_UNSIGNED_BYTE, texture[st].cur->pixels );

  // Setup Texture Params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);       

  glBindTexture(GL_TEXTURE_2D, 0);

  texture[st].dirty = 0;
  }

void SG_TextArea::Disable() {
  flags |= SG_WIDGET_FLAGS_DISABLED;
  state = 1;
  }
 
void SG_TextArea::Enable() {
  flags &= (~SG_WIDGET_FLAGS_DISABLED);
  state = 0;
  }
