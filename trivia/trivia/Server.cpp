#include "Server.h"

void Server::run()
{
	// start communication
	std::thread(&Communicator::startHandleRequests, &this->m_communicator).detach();
	// run until "exit" input
	std::string input = "";
	while(input != "exit")
	{
		std::cin >> input;
	}
	std::cout << "done" << std::endl;
}
