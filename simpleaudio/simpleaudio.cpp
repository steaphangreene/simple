// *************************************************************************
//  This file is part of the SimpleAudio Example Module by Steaphan Greene
//
//  Copyright 2005-2014 Steaphan Greene <steaphan@gmail.com>
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <limits.h>

#include <assert.h>

using namespace std;

#include "simpleaudio.h"

SimpleAudio *SimpleAudio::current = NULL;

//////////////////////////////////////////////Temporary
static bool audio_initialized = false;

#define SOUND_LOOP 1
#define SOUND_TERMINATE 2
#define SOUND_STEREO 4
#define SOUND_MUSIC 128

#define CHANNEL_MUSIC 32767

struct SoundData {
  Uint32 pos;
  Uint32 vol;
  Uint32 pan;
  union {
    Mix_Chunk *sound;
    Mix_Music *music;
  } s;
  Uint32 flags;
  SoundData *next;
};

SoundData SFX[128];
LoadedSound num_sounds = 0;
SoundData *free_blocks = NULL;
SoundData *play_blocks = NULL;
////////////////////////////////////////////////End Temporary

LoadedSound SimpleAudio::BuildSound(const unsigned char *data,
                                    unsigned long len) {
  if (!audio_initialized) return 0;

  SFX[num_sounds].s.sound = Mix_QuickLoad_RAW((Uint8 *)data, len);

  if (!SFX[num_sounds].s.sound) {
    fprintf(stderr, "Warning: Can't build sound!\n");
    return 0;
  }

  ++num_sounds;
  return num_sounds;
}

LoadedSound SimpleAudio::LoadSound(const string &fn) {
  if (!audio_initialized) return 0;

  SFX[num_sounds].s.sound = Mix_LoadWAV(fn.c_str());

  if (!SFX[num_sounds].s.sound) {
    fprintf(stderr, "Warning: Can't load \"%s\"!\n", fn.c_str());
    return 0;
  }

  SFX[num_sounds].flags = 0;

  ++num_sounds;
  return num_sounds;
}

LoadedSound SimpleAudio::LoadMusic(const string &fn) {
  if (!audio_initialized) return 0;
  SFX[num_sounds].s.music = Mix_LoadMUS(fn.c_str());

  if (!SFX[num_sounds].s.music) {
    fprintf(stderr, "Warning: Can't load \"%s\"!\n", fn.c_str());
    return 0;
  }

  SFX[num_sounds].flags = (SOUND_STEREO | SOUND_MUSIC);

  ++num_sounds;
  return num_sounds;
}

SoundData *get_block() {
  SoundData *s;
  if (free_blocks) {
    s = free_blocks;
    free_blocks = s->next;
  } else {
    s = new SoundData;
  }
  return s;
}

PlayingSound SimpleAudio::Play(LoadedSound snd, float vol, float pan) {
  if ((!audio_initialized) || snd < 1) return -1;

  PlayingSound ret;

  if (SFX[snd - 1].flags & SOUND_MUSIC) {
    ret = Mix_PlayMusic(SFX[snd - 1].s.music, 0);
    if (ret >= 0) ret = CHANNEL_MUSIC;
  } else {
    ret = Mix_PlayChannel(-1, SFX[snd - 1].s.sound, 0);
  }
  if (ret >= 0) {
    SetVol(ret, vol);
    SetPan(ret, pan);
  }
  return ret;
}

PlayingSound SimpleAudio::Loop(LoadedSound snd, float vol, float pan,
                               int loops) {
  if ((!audio_initialized) || snd < 1) return -1;

  PlayingSound ret;

  if (SFX[snd - 1].flags & SOUND_MUSIC) {
    ret = Mix_PlayMusic(SFX[snd - 1].s.music, loops);
    // ret = Mix_FadeInMusic(SFX[snd-1].s.music, loops, 100);
    if (ret >= 0) ret = CHANNEL_MUSIC;
  } else {
    ret = Mix_PlayChannel(-1, SFX[snd - 1].s.sound, loops);
  }
  if (ret >= 0) {
    SetVol(ret, vol);
    SetPan(ret, pan);
  }
  return ret;
}

void SimpleAudio::SetVol(PlayingSound s, float vol) {
  if (!audio_initialized || s < 0) return;

  if (s == CHANNEL_MUSIC) {
    Mix_VolumeMusic((int)((float)(MIX_MAX_VOLUME) * vol + 0.5));
  } else {
    Mix_Volume(s, (int)((float)(MIX_MAX_VOLUME) * vol + 0.5));
  }
}

void SimpleAudio::SetPan(PlayingSound s, float pan) {
  if (!audio_initialized || s < 0) return;

  if (s == CHANNEL_MUSIC) return;

  if (pan == 0.0)
    Mix_SetPanning(s, 255, 255);
  else if (pan >= 1.0)
    Mix_SetPanning(s, 0, 255);
  else if (pan <= -1.0)
    Mix_SetPanning(s, 255, 0);
  else if (pan < 0.0)
    Mix_SetPanning(s, 255, (int)(255.0 + pan * 255.0 + 0.5));
  else if (pan > 0.0)
    Mix_SetPanning(s, (int)(255.0 - pan * 255.0 + 0.5), 255);
}

void SimpleAudio::Stop(PlayingSound s) {
  if (!audio_initialized) return;

  if (s == CHANNEL_MUSIC) {
    // Mix_FadeOutMusic(100);
    Mix_HaltMusic();
  } else {
    Mix_HaltChannel(s);
  }
}

SimpleAudio::SimpleAudio(int bufsize) {
  if (current) {
    fprintf(stderr, "ERROR: Created mulitple SimpleAudio instances!\n");
    exit(1);
  }
  current = this;

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, bufsize) < 0) {
    fprintf(stderr, "Error initializing SDL Mixer Audio!\n");
    fprintf(stderr, "Continuing without sound.\n");
    return;
  }
  Mix_AllocateChannels(256);  // FIXME: Make this dynamic
  audio_initialized = true;
}

SimpleAudio::~SimpleAudio() {}
