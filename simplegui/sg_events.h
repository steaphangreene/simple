// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#include "SDL.h"

extern Uint32 SDL_SG_EVENT;

// System Events
#define SG_EVENT_NEEDTORENDER 1  // Something changed but no action

// SG_Widget Actions
#define SG_EVENT_BUTTONCLICK 100
#define SG_EVENT_BUTTONPRESS 101
#define SG_EVENT_BUTTONRELEASE 102
#define SG_EVENT_STICKYOFF 103
#define SG_EVENT_STICKYON 104

// SG_Text Actions
#define SG_EVENT_EDIT 200
#define SG_EVENT_EDITABORT 201
#define SG_EVENT_NEWTEXT 202
#define SG_EVENT_EDITCOMPLETE 202  // Old Name for Event for BW compat.

// SG_MultiText Actions
#define SG_EVENT_SELECTTEXT 300

#define SG_EVENT_MENU 364
#define SG_EVENT_LEFTMENU 365    // These three are the IDs of the
#define SG_EVENT_MIDDLEMENU 366  // PassThrough click menus
#define SG_EVENT_RIGHTMENU 367
#define SG_EVENT_MENUMAX 396  // Max of 32 menu ids (0-31)!

// SG_Ranger Actions
#define SG_EVENT_MOVE 400

// SG_Ranger2D Actions
#define SG_EVENT_MOVE2D 500
#define SG_EVENT_DRAGMOVE 500  // Old Name for Event for BW compat.
#define SG_EVENT_MOVEX 501
#define SG_EVENT_MOVEY 502

#define SG_EVENT_DRAGGRAB 503
#define SG_EVENT_DRAGRELEASE 504

// SG_Alignment Actions (2-Digit Ternary: MSD=Verticle, LSD=Horizontal)
#define SG_EDGE_BEGIN 600  // For Testing Range of Events
#define SG_EDGE_DOWNLEFT 600
#define SG_EDGE_DOWN 601
#define SG_EDGE_DOWNRIGHT 602
#define SG_EDGE_LEFT 603
#define SG_EDGE_NONE 604
#define SG_EDGE_RIGHT 605
#define SG_EDGE_UPLEFT 606
#define SG_EDGE_UP 607
#define SG_EDGE_UPRIGHT 608
#define SG_EDGE_END 609  // For Testing Range of Events

// SG_Passthrough Actions
#define SG_EVENT_CLICK 1000
#define SG_EVENT_LEFTCLICK 1001
#define SG_EVENT_MIDDLECLICK 1002
#define SG_EVENT_RIGHTCLICK 1003
#define SG_EVENT_SCROLLUP 1004
#define SG_EVENT_SCROLLDOWN 1005
#define SG_EVENT_CLICKMAX 1032  // Max of 31 mouse buttons!

#define SG_EVENT_BOX 1032
#define SG_EVENT_LEFTBOX 1033
#define SG_EVENT_MIDDLEBOX 1034
#define SG_EVENT_RIGHTBOX 1035
#define SG_EVENT_BOXMAX 1064  // Max of 31 mouse buttons!

#define SG_EVENT_MOTION 1099  // Mouse Motion

// SG_Compound Actions
#define SG_EVENT_OK 2000
#define SG_EVENT_SELECT 2001
#define SG_EVENT_CANCEL 2002

// SG_FileBrowser Actions
#define SG_EVENT_FILEOPEN 2101

// SG_DNDBoxes Actions
#define SG_EVENT_DND 2900
#define SG_EVENT_DNDDENIED 2901

#define SG_EVENT_SELECT_ITEM 3000
#define SG_EVENT_UNSELECT_ITEM 3001

union SG_Event_DataType {  // Pointer to this static is what is put
  float f[8];              //  in event.data2
  int i[8];
};
