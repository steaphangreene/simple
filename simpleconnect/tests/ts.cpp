#if 0
#!/bin/sh
g++ -Wall `sdl-config --cflags` sc_conn.cpp tests/ts.cpp -o serv `sdl-config --libs` -lSDL_net

exit
#endif

#include "../sc_conn.h"

using namespace std;

int main()
{
	int accepted= 0;
	Connection c = Connection();
	c.StartAccepting(5);

	fprintf(stderr, "%d connections\n", accepted);
	while (c.NumConnections() != 10)
	{
		if (accepted != c.NumConnections())
			fprintf(stderr, "%d connections\n", accepted);
	}
	fprintf(stderr, "%d connections\n", accepted);

	return 0;
}
