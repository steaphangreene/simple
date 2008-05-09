#include "../simplenetwork.h"

using namespace std;

int main()
{
	int accepted= 0;
	SimpleNetwork c = SimpleNetwork();
	c.StartAccepting(5);

	fprintf(stderr, "%d connections\n", accepted);
	while (c.NumConnections() != 10)
	{
		if (accepted != c.NumConnections())
		{
			fprintf(stderr, "%d connections\n", accepted);
			accepted = c.NumConnections();
		}
	}
	fprintf(stderr, "%d connections\n", accepted);

	return 0;
}
