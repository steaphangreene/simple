// *************************************************************************
//  This file is adapted from a part of Acid Rain by Steaphan Greene
//
//  Copyright 2002-2005 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef RENDERER_H
#define RENDERER_H

int init_renderer(int xs, int ys, Uint32 bgcolor=0x000000);
int start_scene();
int finish_scene();
void toggle_fullscreen(void);
void resize_display(int, int);

#endif // RENDERER_H
