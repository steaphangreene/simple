// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace std;

#include "../simplemodel.h"

#include "simplevideo.h"
#include "simpleaudio.h"

static vector<int> anims;
static vector<Uint32> times;

static vector<SimpleModel *> mod;
static Uint32 modnum = 0;
static SimpleModel *weap = NULL;

static SimpleTexture *banner = NULL;

static bool verbose = false;

static Uint32 cur_time;

static Uint32 frames_shown = 0;
static Uint32 frames_counted = 0;
static Uint32 last_time = 0;

static void SetAnim(int which, string anim_name = "") {
  static int anim = 0;
  int oldanim = anim;

  if (which == -1)
    ++anim;
  else if (which == -2)
    --anim;

  map<string, int> anim_map = mod[modnum]->GetAnimations();
  if (which < 0) {
    map<string, int>::const_iterator itr = anim_map.begin();
    for (; itr != anim_map.end(); ++itr) {
      if (anim == itr->second) anim_name = itr->first;
    }
    if (anim_name.length() < 1) {
      anim = oldanim;
      return;
    }
  } else if (anim_map.count(anim_name)) {
    anim = mod[modnum]->LookUpAnimation(anim_name);
  } else {
    return;
  }

  if (which == 0) {
    anims[0] = anim;
    anims[1] = anim;
    times[0] = cur_time;
    times[1] = cur_time;
  } else if (which == 1) {
    anims[1] = anim;
    times[1] = cur_time;
  } else {
    anims[0] = anim;
    times[0] = cur_time;
  }
  if (anim_name.length() > 0) banner->SetText(anim_name);
}

int main(int argc, char **argv) {
  int xs = 1024, ys = 1024;
  vector<string> modname;
  const char *weapname = "";
  Uint32 bgcolor = 0x000000;  // Default Background Color is Black

  int barg = 1;
  while (argc > barg &&
         (strcmp(argv[barg], "-p") == 0 || strcmp(argv[barg], "-v") == 0 ||
          strcmp(argv[barg], "-c") == 0 || strcmp(argv[barg], "-w") == 0)) {
    if (argc <= barg + 1) {
      fprintf(stderr, "ERROR: -p/-w/-c requires file argument\n");
      exit(1);
    }
    if (strcmp(argv[barg], "-p") == 0) {
      SimpleModel::AddSourceFile(argv[barg + 1]);
    } else if (strcmp(argv[barg], "-w") == 0) {
      weapname = argv[barg + 1];
    } else if (strcmp(argv[barg], "-c") == 0) {
      int col = SimpleTexture::ColorByName(argv[barg + 1]);
      bgcolor = SimpleTexture::BGColor(col)->r;
      bgcolor <<= 8;
      bgcolor += SimpleTexture::BGColor(col)->g;
      bgcolor <<= 8;
      bgcolor += SimpleTexture::BGColor(col)->b;
    } else if (strcmp(argv[barg], "-v") == 0) {
      verbose = true;
      barg += 1;
      continue;
    }
    barg += 2;
  }

  if ((argc - barg) < 1) {
    fprintf(stderr,
            "USAGE: %s [-p <search_prefix> [...]] [-s <skin>] "
            "<model_file>|<model_dir> ...\n",
            argv[0]);
    exit(1);
  }

  SimpleVideo *video = new SimpleVideo(xs, ys, 0.0);
  SimpleAudio *audio = new SimpleAudio(4096);

  video->SetDown(0.0, 0);
  video->SetAngle(90.0, 0);
  video->SetPosition(2.5, 0.0, 0);
  video->SetZPosition(-0.5, 0);

  SimpleTexture::SetColor(0, 0.25, 0.25, 0.25, 0.75, 0.75, 0.75);
  banner = new SimpleTexture(0);
  banner->SetDefaultFontSize(50);
  banner->type = SIMPLETEXTURE_TRANSCOLOR;
  banner->SetTextAlignment(ST_ALIGN_CENTER);
  banner->SetText("Default Animation");
  banner->SetTextAspectRatio(16.0);

  vector<string> skinname;
  while (argc - barg >= 1) {
    if (argc - barg >= 2 && strcmp(argv[barg], "-s") == 0) {
      skinname.push_back(argv[barg + 1]);
      barg += 2;
    } else if (argc - barg >= 2 && strcmp(argv[barg], "-a") == 0) {
      mod.back()->LoadAnimation(argv[barg + 1]);
      barg += 2;
    } else if (argc - barg >= 2 && strcmp(argv[barg], "-m") == 0) {
      audio->Play(audio->LoadMusic(argv[barg + 1]));
      barg += 2;
    } else {
      fprintf(stderr, "Loading: '%s'\n", argv[barg]);
      mod.push_back(SM_LoadModel(argv[barg], skinname));
      if (!(mod.back())) {
        fprintf(stderr, "ERROR: Model load failed\n");
        exit(1);
      }
      if (strlen(weapname) > 0) {
        weap = SM_LoadModel(weapname);  // FIXME: Weapon Skin?
        if (weap) {
          mod.back()->AttachSubmodel("tag_weapon", weap);
        }
      }
      if (verbose) {
        map<string, int> anim_map = mod.back()->GetAnimations();
        map<string, int>::const_iterator anim = anim_map.begin();
        printf("Model has these animations:\n");
        for (; anim != anim_map.end(); ++anim) {
          printf("\t%s\n", anim->first.c_str());
        }
      }
      ++barg;
    }
  }

  cur_time = SDL_GetTicks();

  anims.push_back(0);
  anims.push_back(0);
  times.push_back(cur_time);
  times.push_back(cur_time);

  {
    map<string, int> anim_map = mod.back()->GetAnimations();
    map<string, int>::const_iterator itr = anim_map.begin();
    for (; itr != anim_map.end(); ++itr) {
      if (0 == itr->second) banner->SetText(itr->first);
    }
  }

  int quit = 0;
  float yaw = 0.0, pitch = 0.0;
  float posx = 2.0, posy = 0.0, posz = 0.0;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      } else if (event.type == SDL_KEYDOWN) {
        int toggle = -1;
        float dist = 1.0;
        if (event.key.keysym.mod & KMOD_SHIFT) {
          if (event.key.keysym.sym == SDLK_F1)
            SetAnim(1, "TORSO_GESTURE");
          else if (event.key.keysym.sym == SDLK_F2)
            SetAnim(1, "TORSO_ATTACK");
          else if (event.key.keysym.sym == SDLK_F3)
            SetAnim(1, "TORSO_ATTACK2");
          else if (event.key.keysym.sym == SDLK_F4)
            SetAnim(1, "TORSO_DROP");
          else if (event.key.keysym.sym == SDLK_F5)
            SetAnim(1, "TORSO_RAISE");
          else if (event.key.keysym.sym == SDLK_F6)
            SetAnim(1, "TORSO_STAND");
          else if (event.key.keysym.sym == SDLK_F7)
            SetAnim(1, "TORSO_STAND2");
        } else if (event.key.keysym.mod & KMOD_CTRL) {
          if (event.key.keysym.sym == SDLK_F1)
            SetAnim(2, "LEGS_WALKCR");
          else if (event.key.keysym.sym == SDLK_F2)
            SetAnim(2, "LEGS_WALK");
          else if (event.key.keysym.sym == SDLK_F3)
            SetAnim(2, "LEGS_RUN");
          else if (event.key.keysym.sym == SDLK_F4)
            SetAnim(2, "LEGS_BACK");
          else if (event.key.keysym.sym == SDLK_F5)
            SetAnim(2, "LEGS_SWIM");
          else if (event.key.keysym.sym == SDLK_F6)
            SetAnim(2, "LEGS_JUMP");
          else if (event.key.keysym.sym == SDLK_F7)
            SetAnim(2, "LEGS_LAND");
          else if (event.key.keysym.sym == SDLK_F8)
            SetAnim(2, "LEGS_JUMPB");
          else if (event.key.keysym.sym == SDLK_F9)
            SetAnim(2, "LEGS_LANDB");
          else if (event.key.keysym.sym == SDLK_F10)
            SetAnim(2, "LEGS_IDLE");
          else if (event.key.keysym.sym == SDLK_F11)
            SetAnim(2, "LEGS_IDLECR");
          else if (event.key.keysym.sym == SDLK_F12)
            SetAnim(2, "LEGS_TURN");
        } else {
          if (event.key.keysym.sym == SDLK_F1)
            SetAnim(0, "BOTH_DEATH1");
          else if (event.key.keysym.sym == SDLK_F2)
            SetAnim(0, "BOTH_DEAD1");
          else if (event.key.keysym.sym == SDLK_F3)
            SetAnim(0, "BOTH_DEATH2");
          else if (event.key.keysym.sym == SDLK_F4)
            SetAnim(0, "BOTH_DEAD2");
          else if (event.key.keysym.sym == SDLK_F5)
            SetAnim(0, "BOTH_DEATH3");
          else if (event.key.keysym.sym == SDLK_F6)
            SetAnim(0, "BOTH_DEAD3");
        }

        if (event.key.keysym.sym == SDLK_ESCAPE)
          quit = 1;
        else if (event.key.keysym.sym == SDLK_RIGHTBRACKET)
          SetAnim(-1);
        else if (event.key.keysym.sym == SDLK_LEFTBRACKET)
          SetAnim(-2);

        else if (event.key.keysym.sym == SDLK_0)
          toggle = 0;
        else if (event.key.keysym.sym == SDLK_1)
          toggle = 1;
        else if (event.key.keysym.sym == SDLK_2)
          toggle = 2;
        else if (event.key.keysym.sym == SDLK_3)
          toggle = 3;
        else if (event.key.keysym.sym == SDLK_4)
          toggle = 4;
        else if (event.key.keysym.sym == SDLK_5)
          toggle = 5;
        else if (event.key.keysym.sym == SDLK_6)
          toggle = 6;
        else if (event.key.keysym.sym == SDLK_7)
          toggle = 7;
        else if (event.key.keysym.sym == SDLK_8)
          toggle = 8;
        else if (event.key.keysym.sym == SDLK_9)
          toggle = 9;

        if (event.key.keysym.mod & KMOD_SHIFT) {
          dist /= 4.0;
        }
        if (event.key.keysym.mod & KMOD_CTRL) {
          dist /= 16.0;
        }
        if (event.key.keysym.mod & KMOD_ALT) {
          dist /= 64.0;
        }

        if (event.key.keysym.sym == SDLK_RIGHT)
          yaw += 8.0 * dist;
        else if (event.key.keysym.sym == SDLK_LEFT)
          yaw -= 8.0 * dist;
        else if (event.key.keysym.sym == SDLK_UP)
          pitch += 8.0 * dist;
        else if (event.key.keysym.sym == SDLK_DOWN)
          pitch -= 8.0 * dist;

        else if (event.key.keysym.sym == SDLK_HOME)
          posy += dist;
        else if (event.key.keysym.sym == SDLK_END)
          posy -= dist;
        else if (event.key.keysym.sym == SDLK_PAGEDOWN)
          posz += dist;
        else if (event.key.keysym.sym == SDLK_PAGEUP)
          posz -= dist;
        else if (event.key.keysym.sym == SDLK_EQUALS)
          posx += dist;
        else if (event.key.keysym.sym == SDLK_MINUS)
          posx -= dist;

        //	else if(event.key.keysym.sym == SDLK_KP6) posy += dist;
        //	else if(event.key.keysym.sym == SDLK_KP4) posy -= dist;
        //	else if(event.key.keysym.sym == SDLK_KP8) posz += dist;
        //	else if(event.key.keysym.sym == SDLK_KP2) posz -= dist;
        //	else if(event.key.keysym.sym == SDLK_KP_PLUS)  posx += dist;
        //	else if(event.key.keysym.sym == SDLK_KP_MINUS) posx -= dist;

        else if (event.key.keysym.sym == SDLK_RETURN) {
          if (modnum < mod.size() - 1) ++modnum;
        } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
          if (modnum > 0) --modnum;
        } else if (event.key.keysym.sym == SDLK_v) {
          video->ToggleVSync();
        } else {
        }
        if (toggle >= 0) {
          if (event.key.keysym.mod & KMOD_SHIFT) toggle += 10;
          if (event.key.keysym.mod & KMOD_CTRL) toggle += 20;
          if (event.key.keysym.mod & KMOD_ALT) toggle += 40;

          fprintf(stderr, "Toggling material %u\n", toggle);
          mod[modnum]->ToggleMaterial(toggle);
        }
      }
    }
    video->StartScene();

    cur_time = SDL_GetTicks();

    double angle = cur_time / 360.0;
    GLfloat light_position[] = {GLfloat(sin(angle)), GLfloat(cos(angle)), 1.0,
                                0.0};
    GLfloat light_ambient[] = {0.1, 0.1, 0.1, 0.0};
    GLfloat light_diffuse[] = {0.3, 0.3, 0.3, 0.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_DEPTH_TEST);

    glTranslatef(posx, posy, posz);
    glRotatef(yaw, 0.0, 0.0, 1.0);
    glRotatef(pitch, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, -2.0);
    mod[modnum]->Render(SDL_GetTicks(), anims, times);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0, 1.0, -11.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBindTexture(GL_TEXTURE_2D, banner->GLTexture());

    glBegin(GL_QUADS);
    glTexCoord2f(banner->ScaleX(0.0), banner->ScaleY(1.0));
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(banner->ScaleX(1.0), banner->ScaleY(1.0));
    glVertex3f(1.0, 0.0, 0.0);
    glTexCoord2f(banner->ScaleX(1.0), banner->ScaleY(0.0));
    glVertex3f(1.0, 1.0, 0.0);
    glTexCoord2f(banner->ScaleX(0.0), banner->ScaleY(0.0));
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    video->FinishScene();

    frames_shown++;
    if (frames_shown % 100 == 0) {
      Uint32 cur_time = SDL_GetTicks();
      printf("FPS: %.2f\n", (float)((frames_shown - frames_counted) * 1000) /
                                (float)(cur_time - last_time));
      frames_counted = frames_shown;
      last_time = cur_time;
    }
  }

  return 0;
}
