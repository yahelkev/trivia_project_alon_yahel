#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser(std::string name) :m_username(name) {};
	const std::string& getUsername();
private:
	std::string m_username;
};

