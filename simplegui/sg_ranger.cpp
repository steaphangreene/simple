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

#include "sg_ranger.h"

SG_Ranger::SG_Ranger(double fac, double off, double mn, double mx) {
  SetLimits(mn, mx);
  SetFactor(fac);
  SetOffset(off);
  }

SG_Ranger::~SG_Ranger() {
  }


void SG_Ranger::SetLimits(double mn, double mx) {
  min = mn;
  max = mx;
  }

void SG_Ranger::SetFactor(double fac) {
  factor = fac;
  }

void SG_Ranger::SetOffset(double off) {
  offset = off;
  }
