// *************************************************************************
//  This file is adapted from a part of Acid Rain by Steaphan Greene
//
//  Copyright 2002-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 3 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  If not, see <http://www.gnu.org/licenses/>.
//
// *************************************************************************

#ifndef AUDIO_H
#define AUDIO_H

class Sound;

void audio_init(int bufsize = 2048);
void audio_play(int snd, int vol, int pan);
Sound *audio_loop(int snd, int vol, int pan);
void audio_stop(Sound *);
void audio_setvol(Sound *, int);
void audio_setpan(Sound *, int);
int audio_buildsound(const unsigned char *, unsigned long);
int audio_loadsound(const char *);
int audio_loadmusic(const char *);

#endif  // AUDIO_H
