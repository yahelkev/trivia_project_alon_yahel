#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <fstream>


#include <string>

int checkLeak()
{
	try
	{
		std::cout << "Starting..." << std::endl;
		WSAInitializer wsa_init;
		Server myServer;
		myServer.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception was thrown in function: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception in main !" << std::endl;
	}
	return 0;
}
int main()
{
	srand(time(NULL));
	std::map<unsigned int, std::string> answers;
	answers[2] = "lol";
	answers[4] = "1232lol";
	json dict;
	for (const auto& answer : answers) {
		dict[std::to_string(answer.first)] = answer.second;
	}
	json content = {
	{"answers", dict}
	};
	std::cout << content;
	//checkLeak();
	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "Memory leaks!\n";
	}
	else
	{
		std::cout << "No leaks\n";
	}
}
