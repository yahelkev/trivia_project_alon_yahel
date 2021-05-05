#pragma once
#include <string>
#include <string.h>
#include <stdlib.h>

class UserStatistics
{
public:
	UserStatistics() = default;
	// constructor from database
	UserStatistics(int argc, char** argv, char** cols);
	// getters
	float getAverageAnswerTime() const;
	int getCorrectAnswers() const;
	int getTotalAnswers() const;
	int getGameCount() const;

	UserStatistics& operator=(const UserStatistics& other);
private:
	std::string _username;
	float _averageAnswerTime;
	int _correctAnswers;
	int _totalAnswers;
	int _gameCount;
};

