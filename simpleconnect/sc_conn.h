#ifndef SC_CONN_H
#define SC_CONN_H

#include <string>
#include <vector>
#include <list>
#include "safecomm.h"

#include "SDL_thread.h"
#include "SDL_net.h"
using namespace std;

  class Connection {
    public:
	struct Data {
		string playername;
		string password;
		TCPsocket tcp;
		SDL_mutex* recv_mutex;
		vector<Uint8> recv_buffer; // contains buffered data recieved over TCP
		vector<Uint8> send_buffer; // contains buffered sending data.
		unsigned int last_active;
		bool connected;
	};

	Connection(Uint16 port = 4052);
	~Connection(); //cleanup

	// add to send_buffer
	void Add(int, const Uint8&);
        void Add(int, const Uint16&);
	void Add(int, const Uint32&);
	void Add(int, const string&);

	// sends send_buffer over tcp and clears it.
	void Send(int); // should this return int for some reason?

	// returns conection id, adds new connection
	// also for reconnection, with name and password.
	int Connect(IPaddress&, const string& name="", const string& password="");

	void StopAccepting();

	// internals: start this up when a connection drops, takes max amount to accept.
	// when we recieve this amount, stop accepting
	void StartAccepting(int);

	// returns number of connections to the server.
	int NumConnections();

	// disconnects an individual client from the server.
	void Disconnect(int);

	// these recieve data from the recv_buffer and take it off the queue.
	void Recv(int, Uint8&);
	void Recv(int, Uint16&);
	void Recv(int, Uint32&);
        void Recv(int, string&);

	// gets the size of the recv_buffer.
	int RecvBufferSize(int);

	// sets port to given value
	void SetPort(Uint16);

	// set the playername of the given id.
	void SetName(int, const string&);

	// set the password of the given id.
	void SetPassword(int, const string&);

	// gets the playername of the given id.
	string GetName(int);

	// returns 1 if connection is active, 0 otherwise.
	int IsConnected(int);
    private:
	Uint16 port;
	vector<Data> data;
	SDLNet_SocketSet cnx_set;
	int accept_amount; // number of accepted sockets left for StartAccepting()
	TCPsocket sd; // server tcp socket.
	static int RunClient(void*); // client thread.
	bool client_running;
	static int RunServer(void*); // server thread.
	bool server_running;
	static int RunAccept(void*); // runs the accept routine.
	bool accept_running;
    protected:
	SDL_Thread* accept_thread;
	SDL_Thread* client_thread;
	SDL_Thread* server_thread;
	SDL_mutex * data_mutex;
  };

#endif // SC_CONN_H

