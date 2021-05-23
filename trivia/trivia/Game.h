#pragma once
#include <vector>
#include <map>
#include "Room.h"
#include "Question.h"
#include "MessageStructs.h"

class Game
{
public:
	Game(Room&, std::list<Question>);

	Question getQuestionForUser(LoggedUser);
	// submits answer and returns whether it was correct or not
	bool submitAnswer(LoggedUser, int answer);
	void removePlayer(LoggedUser);
	bool gameFinished();
	bool playersLeft();
private:
	std::list<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;

	unsigned int _playersPlaying;
	unsigned int _playersConnected;
};

