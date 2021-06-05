#pragma once
#include <string>
#include "Constants.h"

class GameResults
{
public:
	GameResults(std::string usernam, unsigned int correctAnswerCount, unsigned int wrongAnswerCount, float averageAnswerTime);

	// compare two results by scoring function
	int score() const;
	bool operator<(const GameResults& other) const;
private:
	std::string _username;
	unsigned int _correctAnswerCount;
	unsigned int _wrongAnswerCount;
	float _averageAnswerTime;
};

