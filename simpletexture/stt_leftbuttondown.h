// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleTexture is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleTexture is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleTexture (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef STT_LEFTBUTTONDOWN_H
#define STT_LEFTBUTTONDOWN_H

#include "stt_button.h"

class STT_LeftButton_Down : public STT_Button {
 public:
  STT_LeftButton_Down(const SDL_Color &txtcol);

 protected:
  virtual SDL_Surface *BuildTexture(SDL_Surface *surf, const Uint32 xsize,
                                    const Uint32 ysize, const SDL_Color &col);
  SDL_Color textcol;
};

#endif  // STT_LEFTBUTTONDOWN_H
