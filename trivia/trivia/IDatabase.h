#pragma once
#include <string>
#include <list>
#include "Question.h"

class IDatabase
{
public:
	virtual bool doesUserExist(std::string username) = 0;
	virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
	virtual bool addNewUser(std::string username, std::string password, std::string email) = 0;
	// function returns a list of random questions
	virtual std::list<Question> getQuestions(int questionCount) = 0;
};
