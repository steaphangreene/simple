// *************************************************************************
//  This file is adapted from a part of Acid Rain by Steaphan Greene
//
//  Copyright 2002-2005 Steaphan Greene <stea@cs.binghamton.edu>
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

#ifndef RENDERER_H
#define RENDERER_H

int init_renderer(int, int);
int start_scene(int);
int finish_scene(int);
void toggle_fullscreen(void);
void resize_display(int, int);
void pixels_to_location(double *, double *);
void toggle_settings_window(void);

typedef struct _viewport {
  double xoff, yoff;
  double xtarg, ytarg;
  int movet, move;
  long long data;
  double spread;
  } viewport;

/* For use internally by renderer only! */
void load_xpm_texture(unsigned int tex, char *xpm[]);

#endif /* RENDERER_H */
