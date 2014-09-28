// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#ifndef	SG_RANGER2_H
#define	SG_RANGER2_H

#include "sg_ranger.h"

class SG_Ranger2D {
public:
  SG_Ranger2D(float xspn = 0.0, float yspn = 0.0,
	float xval = 0.0, float yval = 0.0,
	float xmin = 0.0, float ymin = 0.0,
	float xmax = 1.0, float ymax = 1.0);
  virtual ~SG_Ranger2D();

  void SetSpans(float xspn, float yspn);
  void SetXSpan(float xspn);
  void SetYSpan(float yspn);
  void SetValues(float xval, float yval);
  void SetXValue(float xval);
  void SetYValue(float yval);
  void SetLimits(float xmn, float ymn, float xmx, float ymx);
  void SetXLimits(float xmn, float xmx);
  void SetYLimits(float ymn, float ymx);
  void SetActive(float xst, float yst, float xen, float yen);
  void SetXActive(float xst, float xen);
  void SetYActive(float yst, float yen);
  void SetXIncrement(float i);
  void XIncrement();
  void XDecrement();
  void SetYIncrement(float i);
  void YIncrement();
  void YDecrement();

  float XSpan() { return xranger.Span(); };
  float YSpan() { return yranger.Span(); };
  float XValue() { return xranger.Value(); };
  float YValue() { return yranger.Value(); };
  float XMin() { return xranger.Min(); };
  float YMin() { return yranger.Min(); };
  float XMax() { return xranger.Max(); };
  float YMax() { return yranger.Max(); };

  void LinkTo(SG_Ranger2D *other);
  void LinkXTo(SG_Ranger *other);
  void LinkXTo(SG_Ranger2D *other);
  void LinkYTo(SG_Ranger *other);
  void LinkYTo(SG_Ranger2D *other);

  void LinkFrom(SG_Ranger2D *other);
  void LinkXFrom(SG_Ranger *other);
  void LinkXFrom(SG_Ranger2D *other);
  void LinkYFrom(SG_Ranger *other);
  void LinkYFrom(SG_Ranger2D *other);

  SG_Ranger *XRanger() { return &xranger; };
  SG_Ranger *YRanger() { return &yranger; };

  virtual void RangerChanged();

protected:
  SG_Ranger xranger, yranger;
  };

#endif	//SG_RANGER2_H
