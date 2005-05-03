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
static int audio_initialized = 0;

#define SOUND_LOOP	1
#define SOUND_TERMINATE	2
#define SOUND_STEREO	4

struct Sound {
  Uint32 pos;
  Uint32 vol;
  Uint32 pan;
  Mix_Chunk *sound;
  Uint32 flags;
  Sound *next;
  };

Sound SFX[128];
int num_sounds = 0;
Sound *free_blocks = NULL;
Sound *play_blocks = NULL;
////////////////////////////////////////////////End Temporary

/*	DISABLED!
void SimpleAudio::Callback(void *userdata, Uint8 *stream, int len) {

  if(play_blocks == NULL)  {
    memset(stream, 0, len);
    return;
    }

  for(int ctr=0; ctr<len; ctr+=4) {
    long buf_l = 0, buf_r = 0;
    Sound **sptr = &play_blocks;
    while((*sptr) != NULL) {
      if((*sptr)->flags & SOUND_TERMINATE) {
	Sound *it = (*sptr);
	(*sptr) = (*sptr)->next;
	it->next = free_blocks;
	free_blocks = it;
	continue;
	}
      if((*sptr)->vol > 0) {
	if((*sptr)->flags & SOUND_STEREO) { // Stereo Source
	  if((*sptr)->pos+ctr >= (*sptr)->length) {
	    sptr = &((*sptr)->next);
	    continue;
	    }
	  long val_l = Sint8((*sptr)->data[(*sptr)->pos+ctr+1]);
	  val_l <<= 8;
	  val_l |= (*sptr)->data[(*sptr)->pos+ctr+0] & 0xFF;
	  buf_l += val_l * (*sptr)->vol * 16;
	  long val_r = Sint8((*sptr)->data[(*sptr)->pos+ctr+3]);
	  val_r <<= 8;
	  val_r |= (*sptr)->data[(*sptr)->pos+ctr+2] & 0xFF;
	  buf_r += val_r * (*sptr)->vol * 16;
	  }
        else { // Mono Source with software panning
	  if((*sptr)->pos+(ctr>>1) >= (*sptr)->length) {
	    sptr = &((*sptr)->next);
	    continue;
	    }
	  long val = Sint8((*sptr)->data[(*sptr)->pos+(ctr>>1)+1]);
	  val <<= 8;
	  val |= (*sptr)->data[(*sptr)->pos+(ctr>>1)+0] & 0xFF;
	  buf_l += val * (*sptr)->vol * (16-(*sptr)->pan);
	  buf_r += val * (*sptr)->vol * (16+(*sptr)->pan);
	  }
	}

      sptr = &((*sptr)->next);
      }
    if(buf_l > SHRT_MAX<<8) buf_l = SHRT_MAX<<8;
    if(buf_l < SHRT_MIN<<8) buf_l = SHRT_MIN<<8;
    if(buf_r > SHRT_MAX<<8) buf_r = SHRT_MAX<<8;
    if(buf_r < SHRT_MIN<<8) buf_r = SHRT_MIN<<8;
    stream[ctr+0] = (buf_l >> 8);
    stream[ctr+1] = (buf_l >> 16);
    stream[ctr+2] = (buf_r >> 8);
    stream[ctr+3] = (buf_r >> 16);
    }

  Sound **sptr = &play_blocks;
  while((*sptr) != NULL) {
    if((*sptr)->flags & SOUND_STEREO) (*sptr)->pos += len;
    else (*sptr)->pos += len>>1;
    if((*sptr)->pos >= (*sptr)->length) {
      if((*sptr)->flags & SOUND_LOOP) (*sptr)->pos = 0;
      else {
	Sound *it = (*sptr);
	(*sptr) = (*sptr)->next;
	it->next = free_blocks;
	free_blocks = it;
	continue;
	}
      }
    sptr = &((*sptr)->next);
    }
  }
*/

int SimpleAudio::BuildSound(const unsigned char *data, unsigned long len) {
  if(!audio_initialized) return 0;

  SFX[num_sounds].sound = Mix_QuickLoad_RAW((Uint8*)data, len);

  if(!SFX[num_sounds].sound) {
    fprintf(stderr, "Warning: Can't build sound!\n");
    return 0;
    }

  ++num_sounds;
  return num_sounds;
  }

int SimpleAudio::LoadSound(const string &fn) {
  if(!audio_initialized) return 0;

  SFX[num_sounds].sound = Mix_LoadWAV(fn.c_str());

  if(!SFX[num_sounds].sound) {
    fprintf(stderr, "Warning: Can't load \"%s\"!\n", fn.c_str());
    return 0;
    }

  ++num_sounds;
  return num_sounds;
  }

int SimpleAudio::LoadMusic(const string &fn) {
  int ret = LoadSound(fn.c_str());
  SFX[ret-1].flags |= SOUND_STEREO;
  return ret;
  }

Sound *get_block() {
  Sound *s;
  if(free_blocks) { s = free_blocks; free_blocks = s->next; }
  else { s = new Sound; }
  return s;
  }

PlayingSound SimpleAudio::Play(int snd, float vol, float pan) {
  if((!audio_initialized) || snd < 1) return -1;

  PlayingSound ret = Mix_PlayChannel(-1, SFX[snd-1].sound, 0);
  if(ret >= 0) {
    SetVol(ret, vol);
    SetPan(ret, pan);
    }
  return ret;
  }

PlayingSound SimpleAudio::Loop(int snd, float vol, float pan, int loops) {
  if((!audio_initialized) || snd < 1) return -1;

  PlayingSound ret = Mix_PlayChannel(-1, SFX[snd-1].sound, loops);
  if(ret >= 0) {
    SetVol(ret, vol);
    SetPan(ret, pan);
    }
  return ret;
  }

void SimpleAudio::SetVol(PlayingSound s, float vol) {
  if(!audio_initialized || s < 0) return;

  Mix_Volume(s, (int)((float)(MIX_MAX_VOLUME)*vol + 0.5));
  }

void SimpleAudio::SetPan(PlayingSound s, float pan) {
  if(!audio_initialized || s < 0) return;

  if(pan == 0.0) Mix_SetPanning(s, 255, 255);
  else if(pan >=  1.0) Mix_SetPanning(s, 0, 255);
  else if(pan <= -1.0) Mix_SetPanning(s, 255, 0);
  else if(pan < 0.0) Mix_SetPanning(s, 255, (int)(255.0 + pan * 255.0 + 0.5));
  else if(pan > 0.0) Mix_SetPanning(s, (int)(255.0 - pan * 255.0 + 0.5), 255);
  }

void SimpleAudio::Stop(PlayingSound s) {
  if(!audio_initialized) return;

  Mix_HaltChannel(s);
  }

SimpleAudio::SimpleAudio(int bufsize) {
  if(current) {
    fprintf(stderr, "ERROR: Created mulitple SimpleAudio instances!\n");
    exit(1);
    }
  current = this;

  if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, bufsize) < 0) {
    fprintf(stderr, "Error initializing SDL Mixer Audio!\n");
    fprintf(stderr, "Continuing without sound.\n");
    return;
    }
  Mix_AllocateChannels(16);	//FIXME: Make this dynamic
  audio_initialized = 1;
  }
