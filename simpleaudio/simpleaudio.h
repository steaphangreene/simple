// *************************************************************************
//  This file is part of the SimpleAudio Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleAudio is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleAudio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleAudio (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef SIMPLEAUDIO_H
#define SIMPLEAUDIO_H

#include <string>
using namespace std;

#include "SDL.h"
#include "SDL_mixer.h"

typedef int PlayingSound;
typedef int LoadedSound;

class SimpleAudio {
 public:
  SimpleAudio(int bufsize = 512);
  ~SimpleAudio();

  PlayingSound Play(LoadedSound snd, float vol = 1.0, float pan = 0.0);
  PlayingSound Loop(LoadedSound snd, float vol = 1.0, float pan = 0.0,
                    int loops = -1);
  void Stop(PlayingSound);

  void SetVol(PlayingSound, float vol = 1.0);
  void SetPan(PlayingSound, float pan = 0.0);

  LoadedSound BuildSound(const unsigned char *, unsigned long);
  LoadedSound LoadSound(const string &);
  LoadedSound LoadMusic(const string &);

  static SimpleAudio *Current() { return current; };

 private:
  static SimpleAudio *current;
  //  static void Callback(void *userdata, Uint8 *stream, int len);
};

#endif  // AUDIO_H
