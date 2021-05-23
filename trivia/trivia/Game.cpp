#include "Game.h"

Game::Game(Room& room, std::list<Question> questions): m_questions(questions)
{
	// set players map
	for (const LoggedUser& user : room.getAllUsers())
		this->m_players[user] = GameData{ this->m_questions.begin(), 0, 0, 0 };
	// set player counts
	this->_playersConnected = this->_playersPlaying = room.getAllUsers().size();
}

Question Game::getQuestionForUser(LoggedUser user)
{
	return *this->m_players[user].currentQuestion;
}

bool Game::submitAnswer(LoggedUser user, int answer)
{
	GameData& userData = this->m_players[user];
	bool isCorrect = userData.currentQuestion->getCorrectAnswer() == answer;
	// increment question and check if player finished all questions
	if (++userData.currentQuestion == this->m_questions.end())
		this->_playersPlaying--;	// finished playing
	
	(isCorrect ? userData.correctAnswerCount : userData.wrongAnswerCount)++;	// increment correct/wrong answer count
	return isCorrect;
}

void Game::removePlayer(LoggedUser user)
{
	this->_playersConnected--;
	if (this->m_players[user].currentQuestion != this->m_questions.end())	// disconnected in the middle of the game
		this->_playersPlaying--;
}

bool Game::gameFinished()
{
	return this->_playersPlaying == 0;
}

bool Game::playersLeft()
{
	return this->_playersConnected != 0;
}
