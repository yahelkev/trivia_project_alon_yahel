#include "UserStatistics.h"

UserStatistics::UserStatistics(int argc, char** argv, char** cols)
{
	// go over args
	for (int i = 0; i < argc; i++)
	{
		// insert into fields
		if (strcmp(cols[i], "average_answer_time") == 0)
		{
			this->_averageAnswerTime = atof(argv[i]);
		}
		else if (strcmp(cols[i], "correct_answers") == 0)
		{
			this->_averageAnswerTime = atoi(argv[i]);
		}
		else if (strcmp(cols[i], "total_answers") == 0)
		{
			this->_totalAnswers = atoi(argv[i]);
		}
		else if (strcmp(cols[i], "game_count") == 0)
		{
			this->_gameCount = atoi(argv[i]);
		}
		else if (strcmp(cols[i], "username") == 0)
		{
			this->_username = argv[i];
		}
	}
}

float UserStatistics::getAverageAnswerTime() const
{
	return this->_averageAnswerTime;
}

int UserStatistics::getCorrectAnswers() const
{
	return this->_correctAnswers;
}

int UserStatistics::getTotalAnswers() const
{
	return this->_totalAnswers;
}

int UserStatistics::getGameCount() const
{
	return this->_gameCount;
}

UserStatistics& UserStatistics::operator=(const UserStatistics& other)
{
	this->_averageAnswerTime = other._averageAnswerTime;
	this->_correctAnswers = other._correctAnswers;
	this->_gameCount = other._gameCount;
	this->_totalAnswers = other._totalAnswers;
	return *this;
}
