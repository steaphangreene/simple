#if 0
#!/bin/sh
g++ -Wall `sdl-config --cflags` sc_conn.cpp tests/tc.cpp -o client `sdl-config --libs` -lSDL_net

exit
#endif

#include "../sc_conn.h"

using namespace std;

int main()
{
	Connection c = Connection();
	IPaddress ip;
	SDLNet_ResolveHost(&ip, "class11", 4052);
	int slotnum = c.Connect(ip, "Hello", "HI");

	fprintf(stderr, "connected? %d\n", c.IsConnected(slotnum));
	fprintf(stderr, "slotnum: %d\n", c.IsConnected(slotnum));
//	c.Add(slotnum, "myname mypass");
//	c.Send(slotnum);

//	c.Add(slotnum, (Uint16)23);
//	c.Send(slotnum);
	c.Disconnect(slotnum);
	fprintf(stderr, "connected? %d\n", c.IsConnected(slotnum));

	return 0;
}
