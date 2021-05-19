#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase* database) : m_database(database)
{
}

float StatisticsManager::scoreFormula(float averageAnswerTime, int correctAnswers, int totalAnswers, int gameCount)
{
	if (totalAnswers == 0)
		return 0;	// haven't played yet
	return (correctAnswers / totalAnswers) * pow(gameCount, HIGHSCORE_GAME_COUNT_EXPONENT) / (averageAnswerTime + HIGHSCORE_ANSWER_TIME_OFFSET);
}

float StatisticsManager::scoreFormula(const UserStatistics& statistics)
{
	return StatisticsManager::scoreFormula(statistics.getAverageAnswerTime(), statistics.getCorrectAnswers(), statistics.getTotalAnswers(), statistics.getGameCount());
}

std::vector<std::string> StatisticsManager::getHighScores()
{
	std::list<UserStatistics> highscores = this->m_database->getHighScores();
	std::vector<std::string> highscoresDisplay;
	// add rows of all highscores
	for (const UserStatistics& statistics : highscores)
	{
		highscoresDisplay.push_back(statistics.getUsername() + ": " + std::to_string(this->scoreFormula(statistics)));
	}
	return highscoresDisplay;
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username)
{
	if (!this->m_database->doesUserExist(username))
		throw std::exception("User does not exists");

	UserStatistics statistics = this->m_database->getUserStatistics(username);
	std::vector<std::string> statisticsDisplay;
	// add rows of info
	statisticsDisplay.push_back("Game Count:      " + std::to_string(statistics.getGameCount()));
	statisticsDisplay.push_back("Total Answers:   " + std::to_string(statistics.getTotalAnswers()));
	statisticsDisplay.push_back("Correct Answers: " + std::to_string(statistics.getCorrectAnswers()));
	statisticsDisplay.push_back("Average Time:    " + std::to_string(statistics.getAverageAnswerTime()) + "s per question");
	statisticsDisplay.push_back("Score:           " + std::to_string(this->scoreFormula(statistics)));

	return statisticsDisplay;
}
