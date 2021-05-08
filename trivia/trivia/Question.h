#pragma once
#include <string>
#include <vector>

class Question
{
public:
	// constructor from database
	Question(int argc, char** argv, char** cols);
	// getters
	std::string getQuestion() const;
	std::string getPossibleAnswers() const;
	std::string getCorrectAnswer() const;
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswer;
};

