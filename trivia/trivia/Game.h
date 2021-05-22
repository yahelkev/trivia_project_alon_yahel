#pragma once
class Game
{
public:
	Question getQuestionForUser(LoggedUser);
	// submits answer and returns whether it was correct or not
	bool submitAnswer(LoggedUser, int answer);
	void removePlayer(LoggedUser);
private:
	std::vector<Question> m_questions;
	std::map<LoggedUser, GameData> m_players;

	unsigned int _playersPlaying;
	unsigned int _playersConnected;
};

