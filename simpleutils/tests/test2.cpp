#include "../simplenetwork.h"

using namespace std;

int main()
{
	SDL_Init(0);
	int accepted= 0;
	string tmp;
	SimpleNetwork c = SimpleNetwork();
	c.StartAccepting(10);

	fprintf(stderr, "%d connections\n", accepted);
	while (c.NumConnections() < 2)
	{
		if (accepted != c.NumConnections())
		{
			fprintf(stderr, "%d connections\n", accepted);
			accepted = c.NumConnections();
		}
	}
	fprintf(stderr, "%d connections\n", accepted);
	c.StopAccepting();

	SDL_Delay(300);
	c.Recv(0,tmp);
	fprintf(stderr,"tmp: %s\n", tmp.c_str());
	while(true);

	return 0;
}
