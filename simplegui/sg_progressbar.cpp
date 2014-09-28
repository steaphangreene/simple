// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#include "sg_progressbar.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_ProgressBar::SG_ProgressBar(string mes, SimpleTexture c, SimpleTexture dc)
	: SG_TextArea(mes, c) {
  texture.push_back(dc);
  }

SG_ProgressBar::~SG_ProgressBar() {
  }

//  bool SG_ProgressBar::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_ProgressBar::Default_Mouse_Cursor = NULL;


