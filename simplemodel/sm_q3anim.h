// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleModel is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleModel is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleModel (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef	Q3ANIM_H
#define	Q3ANIM_H

enum Q3Animation {
  ANIM_START = 0,

  BOTH_START = ANIM_START,
  BOTH_DEATH1 = BOTH_START,
  BOTH_DEAD1,
  BOTH_DEATH2,
  BOTH_DEAD2,
  BOTH_DEATH3,
  BOTH_DEAD3,
  BOTH_MAX,

  TORSO_START = BOTH_MAX,
  TORSO_GESTURE = TORSO_START,
  TORSO_ATTACK,
  TORSO_ATTACK2,
  TORSO_DROP,
  TORSO_RAISE,
  TORSO_STAND,
  TORSO_STAND2,
  TORSO_MAX,

  LEGS_START = TORSO_MAX,
  LEGS_WALKCR = LEGS_START,
  LEGS_WALK,
  LEGS_RUN,
  LEGS_BACK,
  LEGS_SWIM,
  LEGS_JUMP,
  LEGS_LAND,
  LEGS_JUMPB,
  LEGS_LANDB,
  LEGS_IDLE,
  LEGS_IDLECR,
  LEGS_TURN,
  LEGS_MAX,

  ANIM_MAX = LEGS_MAX
  };

#endif	//Q3ANIM_H
