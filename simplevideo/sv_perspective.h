// *************************************************************************
//  This file is part of the SimpleVideo Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleVideo is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleVideo is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleVideo (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SV_PERSPECTIVE_H
#define SV_PERSPECTIVE_H

#include "sv.h"

class SV_Perspective : public SimpleVideo {
public:
  SV_Perspective(int xs, int ys);
  bool StartScene(double zoom = 4.0, double x = 0.0, double y = 0.0);
  static SV_Perspective *CurrentVideo() { return current; }

protected:
  static SV_Perspective *current;
  };

#endif // SV_PERSPECTIVE_H
