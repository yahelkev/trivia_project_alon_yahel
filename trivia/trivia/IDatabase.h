#pragma once
#include <string>
#include <list>
#include "Question.h"
#include "MessageStructs.h"

class IDatabase
{
public:
	virtual bool doesUserExist(std::string username) = 0;
	virtual bool doesPasswordMatch(std::string username, std::string password) = 0;
	virtual bool addNewUser(std::string username, std::string password, std::string email) = 0;
	// function returns a list of random questions
	virtual std::list<Question> getQuestions(int questionCount) = 0;

	// statistics
	virtual float getAverageAnswerTime(const std::string& username) = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) = 0;
	virtual int getNumOfPlayerGames(const std::string& username) = 0;
	virtual UserStatistics getUserStatistics(const std::string& username) = 0;
	virtual std::list<UserStatistics> getHighScores() = 0;
};
