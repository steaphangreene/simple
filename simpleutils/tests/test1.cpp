#if 0
#!/bin/sh
g++ -Wall `sdl-config --cflags` sc_conn.cpp tests/tc.cpp -o client `sdl-config --libs` -lSDL_net

exit
#endif

#include "../simplenetwork.h"

using namespace std;

int main()
{
	SimpleNetwork c = SimpleNetwork();
	IPaddress ip;
	char str[1024];
	int msgs = 0;
	SDLNet_ResolveHost(&ip, "127.0.0.1", 4052);
	int slotnum = c.Connect(ip, "Hello", "HI");

	fprintf(stderr, "connected? %d\n", c.IsConnected(slotnum));
	fprintf(stderr, "slotnum: %d\n", slotnum);
	c.Add(slotnum, "aword");
	c.Add(slotnum, (Uint8)3);
	c.Send(slotnum);

	while (c.IsConnected(slotnum) == SimpleNetwork::CONN_OK && msgs < 10)
	{
		scanf("%s", (char*)(str));
		c.Add(slotnum, str);
		c.Send(slotnum);
		msgs++;
	}
//	c.Add(slotnum, (Uint16)23);
//	c.Send(slotnum);
	c.Disconnect(slotnum);
	fprintf(stderr, "connected? %d\n", c.IsConnected(slotnum));

	return 0;
}
