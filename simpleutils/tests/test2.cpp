// *************************************************************************
//  This file is part of the SimpleUtils Example Module by Steaphan Greene
//
//  Copyright 2005-2015 Steaphan Greene <steaphan@gmail.com>
//
//  SimpleUtils is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  SimpleUtils is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with SimpleUtils (see the file named "COPYING");
//  if not, write to the the Free Software Foundation, Inc.,
//  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// *************************************************************************

#include "../simplenetwork.h"

using namespace std;

int main(int argc, char **argv) {
  SDL_Init(0);
  int accepted = 0;
  string tmp;
  SimpleNetwork c = SimpleNetwork();
  c.StartAccepting(10);

  fprintf(stderr, "%d connections\n", accepted);
  while (c.NumConnections() < 2) {
    if (accepted != c.NumConnections()) {
      fprintf(stderr, "%d connections\n", accepted);
      accepted = c.NumConnections();
    }
  }
  fprintf(stderr, "%d connections\n", accepted);
  c.StopAccepting();

  SDL_Delay(300);
  c.Recv(0, tmp);
  fprintf(stderr, "tmp: %s\n", tmp.c_str());
  while (true)
    ;

  return 0;
}
