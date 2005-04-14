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

#ifndef	SG_RANGER_H
#define	SG_RANGER_H

class SG_Ranger {
public:
  SG_Ranger(float fac, float off = 0.0, float mn = 0.0, float mx = 1.0);
  ~SG_Ranger();
  void SetFactor(float fac);
  void SetValue(float off);
  void SetLimits(float mn, float mx);

  float Factor() { return factor; };
  float Value() { return offset; };
  float Min() { return min; };
  float Max() { return max; };

protected:
  float factor, offset;
  float min, max;
  };

#endif	//SG_RANGER_H
