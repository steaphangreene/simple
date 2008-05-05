#include <set>
using namespace std;

#include "../sc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_net.h"
#include "SDL_thread.h"

#define MSG_SIZE 1024
#define RETRY_COUNT 30000

TCPsocket sd;
TCPsocket SDLNet_Connect(IPaddress);


int main(int argc, char **argv)
{
	char written_msg[256];
	int PortNumber = 2000;
	IPaddress ip;	/* Server address */

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, argv[1], PortNumber) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "TCP_OPEN");
		exit(EXIT_FAILURE);
	}

	if (strcmp(argv[2],"1")) {
		Sint8 key [] = {66, 85, 76, 76, 79, 88, 32, 72, 69, 76, 76, 79, 33, 0};

		SDLNet_TCP_Send(sd, (void*)key, 14);
	}
	else
		SDLNet_TCP_Send(sd, (void*)"HELLO!", 14);

	SimpleConnect::Connection * conn = new SimpleConnect::Connection(sd);	

	while (conn->is_connected())
	{
		printf("USER> ");
		scanf("%s", written_msg);
		//int len = strlen(written_msg)+1;
		Uint32 rec;

		string s(written_msg);
		//Uint8 tmp = (Uint8) atoi(written_msg);
		conn->Add(written_msg);
		conn->Send();
		fprintf(stderr,"here!\n");
		conn->Recv(rec);
		fprintf(stderr,"there!\n");
		if (s != "")
			fprintf(stderr, "recvd: %X\n", rec);
		fprintf(stderr,"everywhere!\n");


/*
		if ( SDLNet_TCP_Send(sd, (void*)written_msg, len) < len)
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			break;
		}
*/
//		if(strcmp(written_msg, "QUIT") == 0)
//		// Quit the program
//		{
//			printf("will close socket\n");
//			break;
//		}
	}

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
	
	return 0;
};

TCPsocket SDLNet_Connect(IPaddress ip)
{
	//TCPsocket sd;
	int failed = 0;
	/* Open a connection with the IP provided (listen on the host's port) */
	while (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_Connecting: %s \n", SDLNet_GetError());
		failed++;
		if(failed == RETRY_COUNT)
		{
			return NULL;
		}
	}
	return sd;
}

