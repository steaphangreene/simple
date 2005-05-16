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
	: xranger(xspn, xval, xmin, xmax, this),
	  yranger(yspn, yval, ymin, ymax, this) {
  }

SG_Ranger2D::~SG_Ranger2D() {
  }

void SG_Ranger2D::SetSpans(float xspn, float yspn) {
  xranger.SetSpan(xspn);
  yranger.SetSpan(yspn);
  RangerChanged();
  }

void SG_Ranger2D::SetXSpan(float xspn) {
  xranger.SetSpan(xspn);
  RangerChanged();
  }

void SG_Ranger2D::SetYSpan(float yspn) {
  yranger.SetSpan(yspn);
  RangerChanged();
  }

void SG_Ranger2D::SetValues(float xval, float yval) {
  xranger.SetValue(xval);
  yranger.SetValue(yval);
  RangerChanged();
  }

void SG_Ranger2D::SetXValue(float xval) {
  xranger.SetValue(xval);
  RangerChanged();
  }

void SG_Ranger2D::SetYValue(float yval) {
  yranger.SetValue(yval);
  RangerChanged();
  }

void SG_Ranger2D::SetActive(float xst, float yst, float xen, float yen) {
  xranger.SetActive(xst, xen);
  yranger.SetActive(yst, yen);
  RangerChanged();
  }

void SG_Ranger2D::SetXActive(float xst, float xen) {
  xranger.SetActive(xst, xen);
  RangerChanged();
  }

void SG_Ranger2D::SetYActive(float yst, float yen) {
  yranger.SetActive(yst, yen);
  RangerChanged();
  }

void SG_Ranger2D::SetLimits(float xmn, float ymn, float xmx, float ymx) {
  xranger.SetLimits(xmn, xmx);
  yranger.SetLimits(ymn, ymx);
  RangerChanged();
  }

void SG_Ranger2D::SetXLimits(float xmn, float xmx) {
  xranger.SetLimits(xmn, xmx);
  RangerChanged();
  }

void SG_Ranger2D::SetYLimits(float ymn, float ymx) {
  yranger.SetLimits(ymn, ymx);
  RangerChanged();
  }

void SG_Ranger2D::LinkTo(SG_Ranger2D *other) {
  xranger.LinkTo(&(other->xranger));
  yranger.LinkTo(&(other->yranger));
  }

void SG_Ranger2D::LinkXTo(SG_Ranger2D *other) {
  xranger.LinkTo(&(other->xranger));
  }

void SG_Ranger2D::LinkYTo(SG_Ranger2D *other) {
  yranger.LinkTo(&(other->yranger));
  }

void SG_Ranger2D::LinkXTo(SG_Ranger *other) {
  xranger.LinkTo(other);
  }

void SG_Ranger2D::LinkYTo(SG_Ranger *other) {
  yranger.LinkTo(other);
  }

void SG_Ranger2D::LinkFrom(SG_Ranger2D *other) {
  xranger.LinkFrom(&(other->xranger));
  yranger.LinkFrom(&(other->yranger));
  }

void SG_Ranger2D::LinkXFrom(SG_Ranger2D *other) {
  xranger.LinkFrom(&(other->xranger));
  }

void SG_Ranger2D::LinkYFrom(SG_Ranger2D *other) {
  yranger.LinkFrom(&(other->yranger));
  }

void SG_Ranger2D::LinkXFrom(SG_Ranger *other) {
  xranger.LinkFrom(other);
  }

void SG_Ranger2D::LinkYFrom(SG_Ranger *other) {
  yranger.LinkFrom(other);
  }

void SG_Ranger2D::RangerChanged() {
  }
