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

#ifndef SG_TABLE_H
#define SG_TABLE_H

#include "sg_ranger.h"
#include "sg_ranger2d.h"
#include "sg_alignment.h"

#include <set>
#include <vector>
using namespace std;

struct SG_TableGeometry {
  int xpos, ypos;	//Starting X/Y Position (Cell)
  int xsize, ysize;	//X/Y Size (in Cells)
  };

class SG_Table : public SG_Alignment {
public:
  SG_Table(int xsz, int ysz, float xbor = 0.0, float ybor = 0.0);
  virtual ~SG_Table();
  virtual int HandleEvent(SDL_Event *event, float x, float y);
  virtual bool HandEventTo(SG_Widget *targ, SDL_Event *event, float x, float y);
//  virtual bool SetDefaultCursor(GL_MODEL *cur);
  virtual bool AddWidget(SG_Widget *wid, int x1, int y1, int xs=1, int ys=1);
  virtual bool AddWidget(SG_Widget *wid);
  virtual void RemoveWidget(SG_Widget *wid);

  virtual void ClearCol(int xel);
  virtual void ClearRow(int yel);
  virtual void Resize(int xsz, int ysz);
  virtual void AddCol(int xel);
  virtual void AddRow(int yel);
  virtual void RemoveCol(int xel);
  virtual void RemoveRow(int yel);
  virtual void RemoveCols(const vector<int> &xels);
  virtual void RemoveRows(const vector<int> &yels);
  virtual void Substitute(SG_Widget *oldwid, SG_Widget *newwid);

  virtual void SetAspectRatio(float asp);

  void LinkResize(SG_Ranger2D *ranger);
  void LinkXResize(SG_Ranger2D *ranger);
  void LinkYResize(SG_Ranger2D *ranger);
  void LinkXResize(SG_Ranger *ranger);
  void LinkYResize(SG_Ranger *ranger);

  void SetActive(float xst, float yst, float xen, float yen);

  virtual void RangerChanged();
  
protected:
  virtual bool RenderSelf(unsigned long cur_time);

  const SG_AlignmentGeometry CalcGeometry(
	const vector<SG_TableGeometry>::iterator &wgeom
	);

//  static GL_MODEL Default_Mouse_Cursor;
  int xsize, ysize;	//Geometry of Table
  int xpos, ypos;		//Current X/Y Position
  vector<SG_TableGeometry> wgeom;

  void SendResize();
  void SendXResize();
  void SendYResize();
  SG_Ranger2D ranger;
  };

#endif // SG_TABLE_H
