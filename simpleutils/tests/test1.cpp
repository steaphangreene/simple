#include "../simplenetwork.h"

using namespace std;

int main()
{
	SDL_Init(0);
	SimpleNetwork c = SimpleNetwork();
	IPaddress ip;
	SDLNet_ResolveHost(&ip, "127.0.0.1", 4052);
	int slotnum = c.Connect(ip, "Hello", "HI");
	SDLNet_ResolveHost(&ip, "127.0.0.1", 4052);
	int slotnum2 = c.Connect(ip, "Funk", "ADELiC");

	fprintf(stderr, "connected? %d, %d\n", c.IsConnected(slotnum), c.IsConnected(slotnum2));
	fprintf(stderr, "slotnum: %d, %d\n", slotnum, slotnum2);

	SDL_Delay(30);
	c.Add(slotnum,"Worm");
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
