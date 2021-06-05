#pragma once
#include <string>

class LoggedUser
{
public:
	LoggedUser(std::string name) :m_username(name) {};
	const std::string& getUsername() const;

	bool operator==(const LoggedUser& other);
	bool operator<(const LoggedUser& other) const;
private:
	std::string m_username;
};

