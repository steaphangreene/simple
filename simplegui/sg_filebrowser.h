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

#ifndef SG_FILEBROWSER_H
#define SG_FILEBROWSER_H

#include <string>
using namespace std;

#include "sg_compound.h"

class SG_Button;
class SG_TextArea;

class SG_FileBrowser : public SG_Compound {
public:
  SG_FileBrowser(const string &filter);
  virtual ~SG_FileBrowser();
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool ChildEvent(SDL_Event *event);
  const string &FileName();
  
protected:
//  static GL_MODEL Default_Mouse_Cursor;
  string dir;	//Directory
  string reg;	//Pseudo-Regex of Files
  SG_Button *openb;
  SG_TextArea *dirb;
  SG_TextArea *fileb;
  string filename;
  };

#endif // SG_FILEBROWSER_H
