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
#include "sg_ranger2d.h"

SG_Ranger::SG_Ranger(float spn, float val, float mn, float mx,
	SG_Ranger2D *par) {
  parent = par;
  SetLimits(mn, mx);
  SetSpan(spn);
  SetValue(val);
  }

SG_Ranger::~SG_Ranger() {
  set<SG_Ranger *>::iterator itr = linked.begin();
  for(; itr != linked.end(); ++itr) {
    (*itr)->linked.erase(this);
    }
  }

void SG_Ranger::SetActive(float st, float en) {
  float val = Value();
  if(val + Span() < en) val = en - Span();
  if(val > st) val = st;
  if(val != Value()) {
    SetValue(val);
    }
  }

void SG_Ranger::SetLimits(float mn, float mx) {
  min = mn;
  max = mx;
  SetValue(value);	//To check the limits -vs- value & call RangerChanged()

  set<SG_Ranger *>::iterator itr = linked.begin();
  for(; itr != linked.end(); ++itr) {
    (*itr)->max = max;
    (*itr)->min = min;
    (*itr)->value = value;
    (*itr)->RangerChanged();
    }
  }

void SG_Ranger::SetSpan(float spn) {
  span = spn;
  if(span < 0.0) span = 0.0;
  SetValue(value);	//To check the limits -vs- value & call RangerChanged()

  set<SG_Ranger *>::iterator itr = linked.begin();
  for(; itr != linked.end(); ++itr) {
    (*itr)->span = span;
    (*itr)->value = value;
    (*itr)->RangerChanged();
    }
  }

void SG_Ranger::SetValue(float val) {
  value = val;
  if(min < max) {
    if(value < min) value = min;
    else if(value > max - span) value = max - span;
    }
  else {
    if(value > min) value = min;
    else if(value < max + span) value = max + span;
    }
  RangerChanged();

  set<SG_Ranger *>::iterator itr = linked.begin();
  for(; itr != linked.end(); ++itr) {
    (*itr)->value = value;
    (*itr)->RangerChanged();
    }
  }

void SG_Ranger::LinkTo(SG_Ranger *other) {
  other->linked.insert(this);
  linked.insert(other);
  set<SG_Ranger *>::iterator itr = other->linked.begin();
  for(; itr != other->linked.end(); ++itr) if((*itr) != this) {
    (*itr)->linked.insert(this);
    linked.insert(*itr);

    (*itr)->value = value;
    (*itr)->span = span;
    (*itr)->min = min;
    (*itr)->max = max;
    (*itr)->RangerChanged();
    }
  }

void SG_Ranger::LinkFrom(SG_Ranger *other) {
  set<SG_Ranger *>::iterator itr = other->linked.begin();
  for(; itr != other->linked.end(); ++itr) if((*itr) != this) {
    (*itr)->linked.insert(this);
    linked.insert(*itr);

    }
  other->linked.insert(this);
  linked.insert(other);

  value = other->value;
  span = other->span;
  min = other->min;
  max = other->max;

  RangerChanged();
  }

void SG_Ranger::RangerChanged() {
  if(parent) parent->RangerChanged();
  }
