#pragma once
#include <vector>
#include "LoggedUser.h"
#include "IDatabase.h"

class LoginManager
{
public:
	//LoginManager() = default;
	LoginManager(IDatabase* database) : m_database(database) {};
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	bool login(const std::string& username, const std::string& password);
	bool logout(const std::string& username);
private:
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
};

