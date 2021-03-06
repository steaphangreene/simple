// *************************************************************************
//  This file is part of the SimpleGUI Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
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

#ifndef SG_MULTITEXT_H
#define SG_MULTITEXT_H

#include <string>
#include <vector>
using namespace std;

class SG_MultiText {
 public:
  SG_MultiText(){};
  virtual ~SG_MultiText(){};
  virtual const string &Item(int opt) {
    fprintf(stderr,
            "WARNING: Calling SG_MultiText::Item() - should not be here!\n");
    static string s;
    return s;
  };
  virtual void SetItems(const vector<string> &itms) {
    fprintf(
        stderr,
        "WARNING: Calling SG_MultiText::SetItems() - should not be here!\n");
  };
  virtual int NumItems() {
    fprintf(
        stderr,
        "WARNING: Calling SG_MultiText::NumItems() - should not be here!\n");
    return 0;
  };
};

#endif  // SG_MULTITEXT_H
