// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005 Steaphan Greene <stea@cs.binghamton.edu>
//
//  SimpleGUI is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleGUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleGUI (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include <GL/gl.h>

// Posix REGEX not present in all environments (Windows), can't use it?
// #include <regex.h>

#include <cstring>
#include <dirent.h>
using namespace std;

#include "sg_filebrowser.h"
#include "sg_panel.h"
#include "sg_button.h"
#include "sg_editable.h"
#include "sg_events.h"

SG_FileBrowser::SG_FileBrowser(const string &filter)
	: SG_Compound(8, 5, 0.1, 0.1) {
  background = new SG_Panel(SG_COL_FG);

  openb = new SG_Button("Open", SG_COL_RAISED, SG_COL_LOW);
  AddWidget(openb, 6, 4, 2, 1);

  AddWidget(new SG_Button("Cancel", SG_COL_RAISED, SG_COL_LOW), 0, 4, 2, 1);

  dirb = new SG_TextArea("./", SG_COL_LOW, SG_COL_BG);
  AddWidget(dirb, 1, 1, 6, 1);

  fileb = new SG_Editable("file.txt", SG_COL_LOW, SG_COL_BG, SG_COL_HIGH);
  AddWidget(fileb, 1, 2, 6, 1);

  int pos = 0, npos = -1;
  do {
    pos = npos + 1;
    npos = filter.find('/', pos);
    } while(npos >= 0 && npos < (int)(filter.length()));

  string path = filter.substr(0, pos);
  if(path[0] != '/') path = (string)("./") + path;

  string reg = filter.substr(pos);

  printf("Dir = %s, Reg = %s\n", path.c_str(), reg.c_str());

  dirb->SetText(path);

  unsigned int astpos = reg.find('*');
  if(astpos >= reg.length()) {			//No '*' in filename!
    fileb->SetText(reg);
    }
  else {
    fileb->SetText("<unknown>");

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
      fileb->SetText("<unknown>");
      }
    else {
      struct dirent *ent;
      fileb->SetText("<no files>");
      while((ent = readdir(dir))) {	// Only works with '*' first (eg: *.map)
	if(strlen(ent->d_name) >= reg.length() - 1) {
	  if(!strncmp(ent->d_name + strlen(ent->d_name) - (reg.length() - 1),
		reg.c_str() + 1, reg.length() - 1)) {
	    fileb->SetText(ent->d_name);
	    break;
	    }
	  }
	}
      closedir(dir);
      }
    }
  }

SG_FileBrowser::~SG_FileBrowser() {
  }

bool SG_FileBrowser::ChildEvent(SDL_Event *event) {
  if(event->user.code == SG_EVENT_BUTTONPRESS) {
    if(event->user.data1 == (void *)(openb)) {
      event->user.code = SG_EVENT_FILEOPEN;
      event->user.data1 = (void*)this;
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
