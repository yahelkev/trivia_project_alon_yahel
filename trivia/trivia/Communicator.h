#pragma once

#include <Map>
#include <mutex>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include "IRequestHandler.h"

#define PORT 6666
#define IFACE 0

class Communicator
{
	SOCKET m_serverSocket;
	std::map<SOCKET, IRequestHandler*> m_clients;
	// function prepares listening socket for accepting clients
	void bindAndListen();
	// function handles conversation with a client
	void handleNewClient(SOCKET);
public:
	// function starts communication with clients
	void startHandleRequests();
};

