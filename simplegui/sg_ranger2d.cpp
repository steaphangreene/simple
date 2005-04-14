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

SG_Ranger2D::SG_Ranger2D(float xspn, float yspn, float xval, float yval,
	float xmin, float ymin, float xmax, float ymax)
	: xranger(xspn, xval, xmin, xmax),
	  yranger(yspn, yval, ymin, ymax) {
  }

SG_Ranger2D::~SG_Ranger2D() {
  }

void SG_Ranger2D::SetSpans(float xspn, float yspn) {
  xranger.SetSpan(xspn);
  yranger.SetSpan(yspn);
  }

void SG_Ranger2D::SetXSpan(float xspn) {
  xranger.SetSpan(xspn);
  }

void SG_Ranger2D::SetYSpan(float yspn) {
  yranger.SetSpan(yspn);
  }

void SG_Ranger2D::SetValues(float xval, float yval) {
  xranger.SetValue(xval);
  yranger.SetValue(yval);
  }

void SG_Ranger2D::SetXValue(float xval) {
  xranger.SetValue(xval);
  }

void SG_Ranger2D::SetYValue(float yval) {
  yranger.SetValue(yval);
  }

void SG_Ranger2D::SetLimits(float xmn, float ymn, float xmx, float ymx) {
  xranger.SetLimits(xmn, xmx);
  yranger.SetLimits(ymn, ymx);
  }

void SG_Ranger2D::SetXLimits(float xmn, float xmx) {
  xranger.SetLimits(xmn, xmx);
  }

void SG_Ranger2D::SetYLimits(float ymn, float ymx) {
  yranger.SetLimits(ymn, ymx);
  }

