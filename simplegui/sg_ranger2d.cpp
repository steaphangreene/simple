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

#include "sg_ranger2d.h"

SG_Ranger2D::SG_Ranger2D(float xfac, float yfac, float xoff, float yoff)
	: xranger(xfac, xoff), yranger(yfac, yoff) {
  SetFactors(xfac, yfac);
  SetOffsets(xoff, yoff);
  }

SG_Ranger2D::~SG_Ranger2D() {
  }

void SG_Ranger2D::SetFactors(float xfac, float yfac) {
  xranger.SetFactor(xfac);
  yranger.SetFactor(yfac);
  }

void SG_Ranger2D::SetXFactor(float xfac) {
  xranger.SetFactor(xfac);
  }

void SG_Ranger2D::SetYFactor(float yfac) {
  yranger.SetFactor(yfac);
  }

void SG_Ranger2D::SetOffsets(float xoff, float yoff) {
  xranger.SetOffset(xoff);
  yranger.SetOffset(yoff);
  }

void SG_Ranger2D::SetXOffset(float xoff) {
  xranger.SetOffset(xoff);
  }

void SG_Ranger2D::SetYOffset(float yoff) {
  yranger.SetOffset(yoff);
  }
