#include "Game.h"

Game::Game(Room& room, std::list<Question> questions): m_questions(questions), _timePerQuestion(room.getMetaData().timePerQuestion)
{
	// set players map
	for (const LoggedUser& user : room.getAllUsers())
		this->m_players[user] = GameData{ this->m_questions.begin(), 0, 0, 0 };
	// set player counts
	this->_playersConnected = this->_playersPlaying = room.getAllUsers().size();
}

Question Game::getQuestionForUser(LoggedUser user)
{
	m_players[user].lastQuestionTime = time(nullptr);
	return *this->m_players[user].currentQuestion;
}

int Game::submitAnswer(LoggedUser user, int answer)
{
	time_t currentTime = time(nullptr);
	GameData& userData = this->m_players[user];
	int correctAnswer = userData.currentQuestion->getCorrectAnswer();
	int timeTaken = difftime(currentTime, userData.lastQuestionTime);
	if (timeTaken > _timePerQuestion) { timeTaken = _timePerQuestion; };
	bool isCorrect = (correctAnswer == answer) && timeTaken < this->_timePerQuestion;
	(isCorrect ? userData.correctAnswerCount : userData.wrongAnswerCount)++;	// increment correct/wrong answer count
	userData.averageAnswerTime += timeTaken;
	// increment question and check if player finished all questions
	if (++userData.currentQuestion == this->m_questions.end())
	{
		this->_playersPlaying--;	// finished playing
		// calculate average time
		userData.averageAnswerTime /= (userData.correctAnswerCount + userData.wrongAnswerCount);
	}
	
	return correctAnswer;
}

void Game::removePlayer(LoggedUser user)
{
	this->_playersConnected--;
	if (this->m_players[user].currentQuestion != this->m_questions.end())	// disconnected in the middle of the game
		this->_playersPlaying--;
	// calculate average answer time
	GameData& userData = this->m_players[user];
	if (userData.correctAnswerCount + userData.wrongAnswerCount == 0)
		userData.averageAnswerTime = 0;
	else
		userData.averageAnswerTime /= (userData.correctAnswerCount + userData.wrongAnswerCount);
}

bool Game::gameFinished()
{
	return this->_playersPlaying == 0;
}

bool Game::playersLeft()
{
	return this->_playersConnected != 0;
}

GameData& Game::getGameData(LoggedUser user)
{
	return this->m_players[user];
}

std::vector<GameResults> Game::getResults()
{
	std::vector<GameResults> results;
	if(!this->gameFinished())	// results available only after game is finished
		return results;
	// add results from player map
	for (auto const& pair : this->m_players)
		results.push_back(GameResults(pair.first.getUsername(), pair.second.correctAnswerCount, pair.second.wrongAnswerCount, pair.second.averageAnswerTime));
	std::sort(results.rbegin(), results.rend());

	return results;
}
