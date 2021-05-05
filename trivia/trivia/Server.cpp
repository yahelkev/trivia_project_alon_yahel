#include "Server.h"
#include "StatisticsManager.h"

void Server::run()
{
	// start communication
	std::thread(&Communicator::startHandleRequests, &this->m_communicator).detach();
	StatisticsManager sm(this->m_database);
	sm.getHighScores();
	sm.getUserStatistics("a");
	// run until "exit" input
	std::string input = "";
	while(input != "exit")
	{
		std::cin >> input;
	}
	std::cout << "done" << std::endl;
}