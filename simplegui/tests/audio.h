// *************************************************************************
// This is a component of Acid Rain, Pre ALPHA non-distribution version
//
// -By Insomnia (Steaphan Greene)   (Copyright 2002-2005 Steaphan Greene)
//                   (stea@core.binghamton.edu)
//      No waranty stated or implied, I am not responsible for any damage
// caused directly or indirectly by this software.
//      Permision granted for use/distribution/modification by anyone,
// provided this header remains intact, and modified versions are marked
// as so immediately below this header.
//      Products utilizing the code or methods within may be distributed
// freely along with this licence, but any sales for profit of such products
// must have the author's permission, and may be subject to a royaltee fee.
// *************************************************************************

#ifndef AUDIO_H
#define AUDIO_H

class Sound;

void audio_init(int bufsize=2048);
void audio_play(int snd, int vol, int pan);
Sound *audio_loop(int snd, int vol, int pan);
void audio_stop(Sound *);
void audio_setvol(Sound *, int);
void audio_setpan(Sound *, int);
int audio_buildsound(const unsigned char *, unsigned long);
int audio_loadsound(const char *);
int audio_loadmusic(const char *);

#endif //AUDIO_H
