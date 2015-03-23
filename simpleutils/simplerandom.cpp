// *************************************************************************
//  This file is part of the SimpleUtils Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#include "simplerandom.h"
#include "SDL.h"
#include "SDL_thread.h"
#include <time.h>

#ifndef assert
#define assert(condition, string)                                           \
  {                                                                         \
    if (!(condition)) {                                                     \
      fprintf(stderr, "Error in %s.%d: %s!\n", __FILE__, __LINE__, string); \
      exit(1);                                                              \
    }                                                                       \
  }
#endif

void SimpleRandom::ChangeSeed(int Seed) {
  this->seed = Seed;
  this->InitSeed();
}

int SimpleRandom::GetSeed() { return this->seed; }

void SimpleRandom::Initialize() {
  if (SimpleRandom::mutex != NULL) return;

  SimpleRandom::mutex = SDL_CreateMutex();
}

SimpleRandom::SimpleRandom() {
  SimpleRandom::Initialize();
  assert(SDL_mutexP(SimpleRandom::mutex) != -1,
         "[SimpleUtils] Cannot get lock for sequence number\n")

      this->seed = (int)time(0) + SimpleRandom::sequence++;

  assert(SDL_mutexV(SimpleRandom::mutex) != -1,
         "[SimpleUtils] Cannot unlock mutex for sequence number\n")

      this->InitSeed();
}

SimpleRandom::SimpleRandom(int seed) {
  this->seed = seed;
  this->InitSeed();
}

void SimpleRandom::InitSeed() { this->state[0] = this->state[1] = this->seed; }

int SimpleRandom::Rand(int min, int max) {
  int r = 0;

  assert(max - min <= 32767, "[SimpleUtils] Max - Min > 32767!\n")

      r = ((this->state[0] + this->state[1]) % 32767);
  if (r < 0) r = -r;

  this->state[0] = this->state[1];
  this->state[1] = r;

  if (min == max) {
    return min;
  } else {
    return min + (r % (max - min));
  }
}

int SimpleRandom::sequence = 0;
SDL_mutex* SimpleRandom::mutex = NULL;
