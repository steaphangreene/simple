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

#include "sg_globals.h"
#include "sg_animatedpanel.h"

#include <SDL/SDL_ttf.h>

SG_AnimatedPanel::SG_AnimatedPanel(const vector<int> &txtrs, float mspf)
	: SG_Panel(txtrs[0]) {
  for(int tx=1; tx < int(txtrs.size()); ++tx) {
    texture.push_back(txtrs[tx]);
    }
  speed = mspf;
  }

SG_AnimatedPanel::~SG_AnimatedPanel() {
  }

bool SG_AnimatedPanel::Render(unsigned long cur_time) {
  state = int((float)(cur_time) / speed + 0.5) % texture.size();
  return SG_Panel::Render(cur_time);
  }
//  bool SG_AnimatedPanel::SetDefaultCursor(GL_MODEL *cur);
  
//  static GL_MODEL SG_AnimatedPanel::Default_Mouse_Cursor = NULL;
