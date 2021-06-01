#pragma once
#include <vector>
#include <map>
#include "Room.h"
#include "Question.h"
#include "MessageStructs.h"

class Game
{
public:
	Game() = default;
	Game(Room&, std::list<Question>);

	Question getQuestionForUser(LoggedUser);
	// submits answer and returns the correct answer
	int submitAnswer(LoggedUser, int answer);
	void removePlayer(LoggedUser);
	bool gameFinished();
	bool playersLeft();
	std::vector<GameResults> getResults();
private:
	std::list<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;

	unsigned int _timePerQuestion;

	unsigned int _playersPlaying;
	unsigned int _playersConnected;
};

