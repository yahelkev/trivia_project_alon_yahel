#pragma once

#include <Map>
#include <mutex>
#include <iostream>
#include <WinSock2.h>
#include <thread>
#include "LoginRequestHandler.h"

#define IFACE 0

class Communicator
{
	SOCKET m_serverSocket;
	std::mutex m_clientMapMutex;
	std::map<SOCKET, IRequestHandler*> m_clients;
	// function prepares listening socket for accepting clients
	void bindAndListen();
	// function handles conversation with a client
	void handleNewClient(SOCKET);
	// function adds a new client to the map
	void addClient(SOCKET, IRequestHandler*);
	// function removes an existing client
	void deleteClient(SOCKET);
	// function gets a request from client in the trivia protocol
	RequestInfo getRequest(SOCKET);
	// functions get/set requestHandler from the client map
	IRequestHandler* getRequestHandler(SOCKET);
	void setRequestHandler(SOCKET, IRequestHandler*);
public:
	~Communicator();
	// function starts communication with clients
	void startHandleRequests();
};

