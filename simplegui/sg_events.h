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

#define	SDL_SG_EVENT		(SDL_NUMEVENTS-1)

//Basic Actions
#define	SG_EVENT_BUTTONCLICK	100
#define	SG_EVENT_BUTTONPRESS	101
#define	SG_EVENT_BUTTONRELEASE	102
#define	SG_EVENT_STICKYOFF	103
#define	SG_EVENT_STICKYON	104

#define	SG_EVENT_DRAGGRAB	110
#define	SG_EVENT_DRAGMOVE	111
#define	SG_EVENT_DRAGRELEASE	112

#define	SG_EVENT_EDIT		200
#define	SG_EVENT_EDITCOMPLETE	201
#define	SG_EVENT_EDITABORT	202

//Passthrough Actions
#define	SG_EVENT_CLICK		1000
#define	SG_EVENT_LEFTCLICK	1001
#define	SG_EVENT_MIDDLECLICK	1002
#define	SG_EVENT_RIGHTCLICK	1003
#define	SG_EVENT_SCROLLUP	1004
#define	SG_EVENT_SCROLLDOWN	1005
#define	SG_EVENT_CLICKMAX	1032	//Max of 31 mouse buttons!

#define	SG_EVENT_BOX		1032
#define	SG_EVENT_LEFTBOX	1033
#define	SG_EVENT_MIDDLEBOX	1034
#define	SG_EVENT_RIGHTBOX	1035
#define	SG_EVENT_BOXMAX		1064	//Max of 31 mouse buttons!

#define	SG_EVENT_MENU		1064
#define	SG_EVENT_LEFTMENU	1065
#define	SG_EVENT_MIDDLEMENU	1066
#define	SG_EVENT_RIGHTMENU	1067
#define	SG_EVENT_MENUMAX	1096	//Max of 31 mouse buttons!

//Compound Actions
#define	SG_EVENT_OK		2000
#define	SG_EVENT_FILEOPEN	2001
#define	SG_EVENT_SELECT		2002

