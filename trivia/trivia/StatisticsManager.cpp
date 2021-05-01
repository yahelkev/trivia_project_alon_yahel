#include "StatisticsManager.h"

float StatisticsManager::scoreFormula(float averageAnswerTime, int correctAnswers, int totalAnswers, int gameCount)
{
	if (totalAnswers == 0)
		return 0;	// haven't played yet
	return (correctAnswers / totalAnswers) * pow(gameCount, HIGHSCORE_GAME_COUNT_EXPONENT) / (averageAnswerTime + HIGHSCORE_ANSWER_TIME_OFFSET);
}
