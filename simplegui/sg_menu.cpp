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

#include "sg_menu.h"
#include "sg_globals.h"
#include "sg_events.h"

SG_Menu::SG_Menu(string itms, SG_Texture c, SG_Texture ac)
		: SG_TextArea(itms, c) {
  texture.push_back(ac);
  }

SG_Menu::~SG_Menu() {
  }

//  bool SG_Menu::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_Menu::Default_Mouse_Cursor = NULL;


