#pragma once
#include <regex>
#include <algorithm>
#include <vector>
#include <mutex>
#include <random>
#include "LoggedUser.h"
#include "IDatabase.h"

#define PASSWORD_REGEX "[a-zA-Z0-9!@#$%^&*]{8,}"
#define RAND_PASSWORD_LENGHT 8


class LoginManager
{
public:
	LoginManager(IDatabase* dataBase) : m_database(dataBase) {};
	bool signup(const std::string& username, const std::string& password, const std::string& email);
	bool login(const std::string& username, const std::string& password);
	bool logout(const std::string& username);
	bool changePassword(const std::string& newPassword, const std::string& userName);
	void resetPassword(const std::string& userName);

private:

	std::string randomPassword();
   
	IDatabase* m_database;
	std::vector<LoggedUser> m_loggedUsers;
	std::map<std::string, std::string> _oneTimePassword;
};

