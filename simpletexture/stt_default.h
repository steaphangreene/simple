// *************************************************************************
//  This file is part of the SimpleTexture Example Module by Steaphan Greene
//
//  Copyright 2005-2006 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleTexture is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleTexture is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleTexture (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef STT_DEFAULT_H
#define STT_DEFAULT_H

#include "stt.h"

class STT_Default : public ST_Texturator {
protected:
  virtual SDL_Surface *BuildTexture(
	const Uint32 xsize, const Uint32 ysize, const SDL_Color &col
	);
  };

#endif // STT_DEFAULT_H
