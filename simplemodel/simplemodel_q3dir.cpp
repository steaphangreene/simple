// *************************************************************************
//  This file is part of the SimpleModel Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#include <cstdio>
#include <cstdlib>
using namespace std;

#include "sm_q3anim.h"
#include "simplemodel_q3dir.h"

SimpleModel_Q3Dir::SimpleModel_Q3Dir(
	const string &filenm, const string &defskin) {
  head = NULL;
  torso = NULL;
  Load("", filenm, defskin);
  }

SimpleModel_Q3Dir::SimpleModel_Q3Dir(const string &pack,
	const string &filenm, const string &defskin) {
  head = NULL;
  torso = NULL;
  Load(pack, filenm, defskin);
  }

SimpleModel_Q3Dir::SimpleModel_Q3Dir() {
  head = NULL;
  torso = NULL;
  }

SimpleModel_Q3Dir::~SimpleModel_Q3Dir() {
  if(head) delete head;
  if(torso) delete torso;
  }

bool SimpleModel_Q3Dir::Load(const string &pack, const string &filenm, const string &defskin) {
  string base_filename = filenm;
  string legfile = base_filename + "/lower.md3";
  string torsofile = base_filename + "/upper.md3";
  string headfile = base_filename + "/head.md3";

  SimpleModel_MD3::Load(pack, legfile, defskin);

  torso = new SimpleModel_MD3(pack, torsofile, defskin);
  SetAnimOffset("tag_torso", 1);
  if(torso) {
    Uint32 tnum = TagNameToIndex("tag_torso");
    AttachSubmodel(tnum, torso);
    target_models.insert(tnum);
    }

  head = new SimpleModel_MD3(pack, headfile, defskin);
  if(torso && head) torso->AttachSubmodel("tag_head", head);

  if(pack.length() > 0) {
    if(!LoadCFG(pack + "/" + base_filename + "/animation.cfg")) return false;
    }
  else {
    if(!LoadCFG(base_filename + "/animation.cfg")) return false;
    }

  animations["BOTH_DEATH1"] = BOTH_DEATH1;
  animations["BOTH_DEAD1"] = BOTH_DEAD1;
  animations["BOTH_DEATH2"] = BOTH_DEATH2;
  animations["BOTH_DEAD2"] = BOTH_DEAD2;
  animations["BOTH_DEATH3"] = BOTH_DEATH3;
  animations["BOTH_DEAD3"] = BOTH_DEAD3;
  animations["TORSO_GESTURE"] = TORSO_GESTURE;
  animations["TORSO_ATTACK"] = TORSO_ATTACK;
  animations["TORSO_ATTACK2"] = TORSO_ATTACK2;
  animations["TORSO_DROP"] = TORSO_DROP;
  animations["TORSO_RAISE"] = TORSO_RAISE;
  animations["TORSO_STAND"] = TORSO_STAND;
  animations["TORSO_STAND2"] = TORSO_STAND2;
  animations["LEGS_WALKCR"] = LEGS_WALKCR;
  animations["LEGS_WALK"] = LEGS_WALK;
  animations["LEGS_RUN"] = LEGS_RUN;
  animations["LEGS_BACK"] = LEGS_BACK;
  animations["LEGS_SWIM"] = LEGS_SWIM;
  animations["LEGS_JUMP"] = LEGS_JUMP;
  animations["LEGS_LAND"] = LEGS_LAND;
  animations["LEGS_JUMPB"] = LEGS_JUMPB;
  animations["LEGS_LANDB"] = LEGS_LANDB;
  animations["LEGS_IDLE"] = LEGS_IDLE;
  animations["LEGS_IDLECR"] = LEGS_IDLECR;
  animations["LEGS_TURN"] = LEGS_TURN;

  return false;
  }

bool SimpleModel_Q3Dir::LoadCFG(const string &filenm) {
  SDL_RWops *cfg = SDL_RWFromZZIP(filenm.c_str(), "rb");
  if(!cfg) {
    fprintf(stderr, "WARNING: Could not open model animations file '%s'!\n",
	filenm.c_str());
    return false;
    }

  SDL_RWseek(cfg, 0, SEEK_END);
  int filesz = SDL_RWtell(cfg);
  SDL_RWseek(cfg, 0, SEEK_SET);
  char *filedata = new char[filesz+1]; //FIXME: Handle Error!
  char *fileptr = filedata;
  while(fileptr < (filedata+filesz)) {
    fileptr += SDL_RWread(cfg, fileptr, 1, filesz);
    //FIXME: Handle Error!
    }
  *fileptr = 0;
  SDL_RWclose(cfg);
  fileptr = filedata;

  int start=0, num=0, loop=0, fps=0;
  int torso_first = -1, legs_offset = -1;
  for(int anim = ANIM_START; anim < ANIM_MAX; ++anim) {
    while(sscanf(fileptr, "%d %d %d %d", &start, &num, &loop, &fps) < 4) {
      //Skip irrelevant lines
      while((*fileptr) && (*fileptr) != '\n') ++fileptr;
      if((*fileptr)) ++fileptr;
      }

    //Advance to next line
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && isspace(*fileptr)) ++fileptr;
    while((*fileptr) && isdigit(*fileptr)) ++fileptr;
    while((*fileptr) && (*fileptr) != '\n') ++fileptr;

    if(anim < BOTH_MAX) {
      torso->AddAnimation(start, start + num, loop, fps);
      AddAnimation(start, start + num, loop, fps);
      }
    else if(anim < TORSO_MAX) {
      if(torso_first < 0) torso_first = start;
      torso->AddAnimation(start, start + num, loop, fps);
      }
    else if(anim < LEGS_MAX) {
      if(legs_offset < 0) legs_offset = start - torso_first;
      start -= legs_offset;
      AddAnimation(start, start + num, loop, fps);
      }
    else {
      fprintf(stderr, "WARNING: Too many animations for Q3 in '%s'!\n",
		filenm.c_str());
      }
    }

  delete [] filedata;
  return true;
  }

bool SimpleModel_Q3Dir::RenderSelf(Uint32 cur_time, const vector<int> &anim,
	const vector<Uint32> &start_time, Uint32 anim_offset) const {
  if(anim.size() < 2 || start_time.size() < 2) {
    fprintf(stderr, "WARNING: Not enough anims/times sent to Q3 renderer.\n");
    return false;
    }

/*
  vector<int> leganim(1);
  leganim[0] = anim[0];
  vector<Uint32> legtime(1);
  legtime[0] = start_time[0];

  //Do some auto-correction for not-quite valid inputs
  if(legtime[1] >= legtime[0] && leganim[1] < BOTH_MAX) {
    leganim[0] = leganim[1];
    legtime[0] = legtime[1];
    }
  else if(legtime[0] >= legtime[1] && leganim[0] < BOTH_MAX) {
    leganim[1] = leganim[0];
    legtime[1] = legtime[0];
    }
  else if(leganim[1] < BOTH_MAX) {
    leganim[1] = TORSO_STAND;
    legtime[1] = legtime[0];
    }
  else if(leganim[0] < BOTH_MAX) {
    leganim[0] = LEGS_IDLE;
    legtime[0] = legtime[1];
    }

  if(leganim[0] >= BOTH_MAX) {	//Adjust for Leg Offset
    leganim[0] -= (LEGS_START - TORSO_START);
    }
*/

//  glPushMatrix();
  glColor3f(1.0, 1.0, 1.0);
  glEnable(GL_TEXTURE_2D);
  glScalef(1.0/24.0, 1.0/24.0, 1.0/24.0);
  glTranslatef(0.0, 0.0, 24.0);	//Adjust for floor height

  SimpleModel_MD3::RenderSelf(cur_time, anim, start_time);

//  glPopMatrix();
  return true;
  }

void SimpleModel_Q3Dir::SetWeapon(SimpleModel_MD3 *weap) {
  if(torso) torso->AttachSubmodel("tag_weapon", weap);
  }

const vector<string> &SimpleModel_Q3Dir::GetSkinList() {
  return skins;
  }
