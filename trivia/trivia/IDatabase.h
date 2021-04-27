#pragma once
#include <string>

class IDatabase
{
public:
	// function checks if a user with this username exists in the database
	virtual bool doesUserExist(std::string username) = 0;
	// function checks if the password matches this user's password in the database
	virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
	// function adds a new user to the database
	virtual bool addNewUser(std::string username, std::string password, std::string email) = 0;
};
