// *************************************************************************
//  This file is part of the SimpleUtils Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleUtils is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleUtils is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleUtils (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
// *************************************************************************

#ifndef _RANDOM_H
#define _RANDOM_H
#include "SDL.h"
#include "SDL_thread.h"
#include <time.h>

/*
 * Implements Lagged Fibonacci Generator RNG
 */
class SimpleRandom
{
  private:
  //TODO: int, or uint32?
  int seed;
  int state[2];
  void InitSeed();
  static int sequence;
  static SDL_mutex* mutex;

  public:
  static void Initialize();
  void ChangeSeed(int seed);
  int GetSeed();
  int Rand(int min, int max);

  SimpleRandom();
  SimpleRandom(int seed);
};

#endif
