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
  SG_Ranger2D(double xfac, double yfac, double xoff = 0.0, double yoff = 0.0,
	double xmin = 0.0, double ymin = 0.0,
	double xmax = 1.0, double ymax = 1.0);
  ~SG_Ranger2D();
  void SetFactors(double xfac, double yfac);
  void SetXFactor(double xfac);
  void SetYFactor(double yfac);
  void SetOffsets(double xoff, double yoff);
  void SetXOffset(double xoff);
  void SetYOffset(double yoff);
  void SetLimits(double xmn, double ymn, double xmx, double ymx);
  void SetXLimits(double xmn, double xmx);
  void SetYLimits(double ymn, double ymx);

  double XFactor() { return xranger.Factor(); };
  double YFactor() { return yranger.Factor(); };
  double XOffset() { return xranger.Offset(); };
  double YOffset() { return yranger.Offset(); };
  double XMin() { return xranger.Min(); };
  double YMin() { return yranger.Min(); };
  double XMax() { return xranger.Max(); };
  double YMax() { return yranger.Max(); };

protected:
  SG_Ranger xranger, yranger;
  };

#endif	//SG_RANGER2_H
