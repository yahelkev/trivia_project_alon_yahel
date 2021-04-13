#include "Server.h"

void Server::run()
{
	std::string input = "";
	while(input != "exit")
	{
		std::cin >> input;
	}
	std::cout << "done" << std::endl;
}
