// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2008 Steaphan Greene <stea@cs.binghamton.edu>
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

#include "SDL_opengl.h"

// Posix REGEX not present in all environments (Windows), can't use it?
// #include <regex.h>

#include <cstring>
#include <dirent.h>
using namespace std;

#include "sg_filebrowser.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_editable.h"
#include "sg_combobox.h"
#include "sg_events.h"

SG_FileBrowser::SG_FileBrowser(const string &filter, bool newfile)
	: SG_Compound(12, 10, 0.1, 0.1) {
  background = new SG_Panel(SG_COL_FG);

  openb = new SG_Button("Open", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(openb, 10, 9, 2, 1);

  cancelb = new SG_Button("Cancel", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(cancelb, 0, 9, 2, 1);

  dirb = new SG_TextArea("./", SG_COL_LOW, SG_COL_BG);
  AddWidget(dirb, 1, 1, 10, 1);

  int pos = 0, npos = -1;
  do {
    pos = npos + 1;
    npos = filter.find('/', pos);
    } while(npos >= 0 && npos < (int)(filter.length()));

  string path = filter.substr(0, pos);
  if(path[0] != '/') path = (string)("./") + path;

  string reg = filter.substr(pos);

  dirb->SetText(path);

  vector<string> files;

  unsigned int astpos = reg.find('*');
  if(astpos >= reg.length()) {			//No '*' in filename!
    files.push_back(reg);
    }
  else {
    unsigned int testast = reg.find('*', astpos + 1);

    if(testast < reg.length()) {		//Multiple '*'s in filename!
      fprintf(stderr,
	"WARNING: Unhandled complex regex '%s' in SG_FileBrowser!\n",
	reg.c_str());
      return;
      }

    if(astpos > 0) {			//'*'s not at start (unhandled)!
      fprintf(stderr,
	"WARNING: Unhandled non-simple regex '%s' in SG_FileBrowser!\n",
	reg.c_str());
      return;
      }

    DIR *dir = opendir(path.c_str());
    if(dir == NULL) {
      fprintf(stderr, "WARNING: Unable to open directory '%s'!\n",
	path.c_str());
      }
    else {
      struct dirent *ent;
      while((ent = readdir(dir))) {	// Only works with '*' first (eg: *.map)
	if(strlen(ent->d_name) >= reg.length() - 1) {
	  if(!strncmp(ent->d_name + strlen(ent->d_name) - (reg.length() - 1),
		reg.c_str() + 1, reg.length() - 1)) {
	    files.push_back(ent->d_name);
	    }
	  }
	}
      closedir(dir);
      }
    }
  fileb = new SG_ComboBox(files, false);
  if(files.size() == 0) fileb->SetText("<no files>");
  AddWidget(fileb, 1, 2, 10, 1);
  }

SG_FileBrowser::~SG_FileBrowser() {
  }

bool SG_FileBrowser::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(openb)) {
      event->user.code = SG_EVENT_FILEOPEN;
      event->user.data1 = (void*)(SG_FileBrowser*)this;
      event->user.data2 = NULL;
      return 1;
      }
    else if(event->user.data1 == (void *)(cancelb)) {
      event->user.code = SG_EVENT_CANCEL;
      event->user.data1 = (void*)(SG_FileBrowser*)this;
      event->user.data2 = NULL;
      return 1;
      }
    }
  return 0; // Silence children doing other things
  }

const string &SG_FileBrowser::FileName() {
  filename = dirb->Text() + "/" + fileb->Text();
  return filename;
  }

//  bool SG_FileBrowser::SetDefaultCursor(GL_MODEL *cur);

//  static GL_MODEL SG_FileBrowser::Default_Mouse_Cursor = NULL;
