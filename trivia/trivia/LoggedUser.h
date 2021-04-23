#pragma once
#include <string>

class LoggedUser
{
public:
	const std::string& getUsername();
private:
	std::string m_username;
};

