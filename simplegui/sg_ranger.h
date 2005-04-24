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

#include <set>
using namespace std;

class SG_Ranger2D;

class SG_Ranger {
public:
  SG_Ranger(float spn = 0.0, float val = 0.0, float mn = 0.0, float mx = 1.0,
	SG_Ranger2D *par = NULL);
  virtual ~SG_Ranger();

  void SetValue(float val);
  void SetSpan(float spn);
  void SetLimits(float mn, float mx);

  float Span() { return span; };
  float Value() { return value; };
  float Min() { return min; };
  float Max() { return max; };

  void LinkTo(SG_Ranger *other);
  void LinkFrom(SG_Ranger *other);

  virtual void RangerChanged();

protected:
  float span, value;
  float min, max;
  set<SG_Ranger*> linked;
  SG_Ranger2D *parent;
  };

#endif	//SG_RANGER_H
