#include "GameResults.h"

GameResults::GameResults(std::string username, unsigned int correctAnswerCount, unsigned int wrongAnswerCount, float averageAnswerTime) : _username(username), _correctAnswerCount(correctAnswerCount), _wrongAnswerCount(wrongAnswerCount), _averageAnswerTime(averageAnswerTime)
{
}

int GameResults::score() const
{
	return this->_correctAnswerCount / this->_wrongAnswerCount / (this->_averageAnswerTime + HIGHSCORE_ANSWER_TIME_OFFSET);
}

bool GameResults::operator<(const GameResults& other) const
{
	return this->score() < other.score();
}

std::string GameResults::getUsername()
{
	return this->_username;
}

unsigned int GameResults::getCorrectAnswerCount()
{
	return this->_correctAnswerCount;
}

unsigned int GameResults::getWrongAnswerCount()
{
	return this->_wrongAnswerCount;
}

float GameResults::getAverageAnswerTime()
{
	return this->_averageAnswerTime;
}
