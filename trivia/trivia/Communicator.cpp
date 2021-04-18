#include "Communicator.h"

void Communicator::startHandleRequests()
{
	// create listening socket for accepting clients
	this->m_serverSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
	{
		std::cout << "Server stopped working. Error on socket creation." << std::endl;
		exit(1);
	}
	this->bindAndListen();

	// accept clients
	while (true)
	{
		SOCKET clientSocket = accept(this->m_serverSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "Server stopped working. Error on client accept." << std::endl;
			exit(1);
		}
		// handle client conversation
		std::thread(&Communicator::handleNewClient, this, clientSocket).detach();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	if (::bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	if (::listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	int socketResult = 0;
	this->addClient(clientSocket, new LoginRequestHandler);

	// send hello message
	const char* msg = "hello";
	socketResult = send(clientSocket, msg, strlen(msg), 0);
	if (socketResult == INVALID_SOCKET)
	{
		this->deleteClient(clientSocket);
		return;
	}

	// receive hello message
	char receivedMessage[MESSAGE_LENGTH + 1] = { 0 };
	socketResult = recv(clientSocket, receivedMessage, MESSAGE_LENGTH, 0);
	if (socketResult == INVALID_SOCKET)
	{
		this->deleteClient(clientSocket);
		return;
	}

	// print result and close connection
	std::cout << "received message: " << receivedMessage << std::endl;
	this->deleteClient(clientSocket);
}

void Communicator::addClient(SOCKET clientSocket, IRequestHandler* requestHandler)
{
	std::lock_guard<std::mutex> clientMapLock(this->m_clientMapMutex);
	this->m_clients[clientSocket] = requestHandler;
}

void Communicator::deleteClient(SOCKET clientSocket)
{
	std::lock_guard<std::mutex> clientMapLock(this->m_clientMapMutex);
	delete this->m_clients[clientSocket];
	this->m_clients.erase(clientSocket);
	closesocket(clientSocket);
}

Communicator::~Communicator()
{
	try
	{
		// delete clients left in map
		for (auto const& client : this->m_clients)
		{
			closesocket(client.first);
			delete client.second;
		}
		// close listening socket
		closesocket(this->m_serverSocket);
	}
	catch (...) {}
}
