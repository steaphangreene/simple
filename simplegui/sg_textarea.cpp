// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
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
  dis_tex.LinkTextFrom(&(texture[0]));
  texture.push_back(dis_tex);
  SetText(mes);
  }

SG_TextArea::~SG_TextArea() {
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
  for(int tx=0; tx < int(texture.size()); ++tx) {
    texture[tx].SetMargins(xmar, ymar);
    }
  }

void SG_TextArea::SetText(const string &mes) {
  message = mes;
  texture[0].SetText(message);
  for(int tx=1; tx < int(texture.size()); ++tx) {
    texture[tx].LinkTextFrom(&(texture[0]));
    }
  UpdateRange();
  }

void SG_TextArea::SetFontSize(int sz) {
  texture[0].SetFontSize(sz);
  for(int tx=0; tx < int(texture.size()); ++tx) {
    texture[tx].LinkTextFrom(&(texture[0]));
    }
  UpdateRange();
  }

const string &SG_TextArea::Text() {
  return message;
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
  for(int tx=0; tx < int(texture.size()); ++tx) {
    texture[tx].SetTextVisibleSize(ys, xs);
    }
  UpdateRange();
  }

void SG_TextArea::SetVisibleLines(int numlns) {	//Depricated!
  for(int tx=0; tx < int(texture.size()); ++tx) {
    texture[tx].SetTextVisibleSize(numlns);
    }
  UpdateRange();
  }

void SG_TextArea::UpdateRange() {
  double font_height = TTF_FontHeight(texture[0].CurrentFont());
  const TextGeometry *tgeom = texture[0].GetTextGeometry();

  if(tgeom->visible_xlines > 0) {
    SetXLimits(0.0, (double)(tgeom->text_xsize) / font_height);
    SetXSpan((double)(tgeom->visible_xlines));
    }
  else {
    SetXLimits(0.0, (double)(tgeom->text_xsize) / font_height);
    SetXSpan((double)(tgeom->text_xsize) / font_height);
    }

  if(tgeom->visible_ylines > 0) {	//Allows scrolling before/after text vertically
    SetYLimits(-(double)(tgeom->visible_ylines), (double)(NumLines()) + (double)(tgeom->visible_ylines));
    SetYSpan((double)(tgeom->visible_ylines));
    }
  else {
    SetYLimits(-(double)(NumLines()), (double)(NumLines()) * 2.0);
    SetYSpan((double)(NumLines()));
    }
  }

void SG_TextArea::RangerChanged() {
  for(int tx=0; tx < int(texture.size()); ++tx) {
//    texture[tx].SetTextVisibleSize(YSpan(), XSpan());
	//FIXME: Need to tranmit these changes somehow!

    texture[tx].SetTextPosition(YValue(), XValue());
    }
  }

void SG_TextArea::SetAlignment(int align) {
  for(int tx=0; tx < int(texture.size()); ++tx) {
//    texture[tx].SetAlignment(align);
    }
  }

unsigned int SG_TextArea::NumLines() {
  vector<string> lines;
  int pos = 0, lpos = 0;
  while(lpos < (int)(message.length())) {
    pos = message.find('\n', lpos);
    if(pos < lpos) pos = message.length();
    lines.push_back(message.substr(lpos, pos - lpos));
    lpos = pos+1;
    }
  return lines.size();
  }
