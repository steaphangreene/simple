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

#ifndef	SG_RANGER2_H
#define	SG_RANGER2_H

#include "sg_ranger.h"

class SG_Ranger2D {
public:
  SG_Ranger2D(float xfac, float yfac, float xoff = 0.0, float yoff = 0.0,
	float xmin = 0.0, float ymin = 0.0, float xmax = 1.0, float ymax = 1.0);
  ~SG_Ranger2D();
  void SetFactors(float xfac, float yfac);
  void SetXFactor(float xfac);
  void SetYFactor(float yfac);
  void SetOffsets(float xoff, float yoff);
  void SetXOffset(float xoff);
  void SetYOffset(float yoff);
  void SetLimits(float xmn, float ymn, float xmx, float ymx);
  void SetXLimits(float xmn, float xmx);
  void SetYLimits(float ymn, float ymx);

  float XFactor() { return xranger.Factor(); };
  float YFactor() { return yranger.Factor(); };
  float XOffset() { return xranger.Offset(); };
  float YOffset() { return yranger.Offset(); };
  float XMin() { return xranger.Min(); };
  float YMin() { return yranger.Min(); };
  float XMax() { return xranger.Max(); };
  float YMax() { return yranger.Max(); };

protected:
  SG_Ranger xranger, yranger;
  };

#endif	//SG_RANGER2_H
