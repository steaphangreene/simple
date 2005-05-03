// *************************************************************************
//  This file is part of the SimpleAudio Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleAudio is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleAudio is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleAudio (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#ifndef SIMPLEAUDIO_H
#define SIMPLEAUDIO_H

#include <string>
using namespace std;

#include "SDL_mixer.h"

class Sound;

class SimpleAudio {
public:
  SimpleAudio(int bufsize=2048);
  ~SimpleAudio();
  void Play(int snd, int vol, int pan);
  Sound *Loop(int snd, int vol, int pan);
  void Stop(Sound *);
  void SetVol(Sound *, int);
  void SetPan(Sound *, int);
  int BuildSound(const unsigned char *, unsigned long);
  int LoadSound(const string &);
  int LoadMusic(const string &);

private:
  static void Callback(void *userdata, Uint8 *stream, int len);
  };

#endif //AUDIO_H
