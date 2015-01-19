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
  SimpleNetwork c = SimpleNetwork();
  IPaddress ip;
  SDLNet_ResolveHost(&ip, "127.0.0.1", 4052);
  int slotnum = c.Connect(ip, "Hello", "HI");
  SDLNet_ResolveHost(&ip, "127.0.0.1", 4052);
  int slotnum2 = c.Connect(ip, "Funk", "ADELiC");

  fprintf(stderr, "connected? %d, %d\n", c.IsConnected(slotnum),
          c.IsConnected(slotnum2));
  fprintf(stderr, "slotnum: %d, %d\n", slotnum, slotnum2);

  SDL_Delay(30);
  c.Add(slotnum, "Worm");
  c.Send(slotnum);

  SDL_Delay(3000);
  c.Disconnect(slotnum);
  c.Disconnect(slotnum2);

  SDL_Delay(5000);
  fprintf(stderr, "RESOLVE HOST\n");
  SDLNet_ResolveHost(&ip, "bumblebee.cs.binghamton.edu", 4052);
  slotnum = c.Connect(ip, "Hello", "HI");
  fprintf(stderr, "RECON\n");

  SDL_Delay(40);
  c.Add(slotnum, "FRESH");
  c.Send(slotnum);
  SDL_Delay(4000);

  /*
          while (c.IsConnected(slotnum) == SimpleNetwork::CONN_OK && msgs < 10)
          {
                  scanf("%s", &str);
                  if (msgs % 2 == 0)
                          curslot=slotnum;
                  else
                          curslot=slotnum2;

                  c.Add(slotnum, str);
                  c.Send(slotnum);

                  msgs++;
          }
  */
  c.Disconnect(slotnum);
  c.Disconnect(slotnum2);
  fprintf(stderr, "connected? %d\n", c.IsConnected(slotnum));

  return 0;
}
