#pragma once
#include <string>
#include <vector>

class Question
{
public:
	// constructor from database
	Question(int argc, char** argv, char** cols);
	Question(int correctAnswer, std::string question, std::vector<std::string> answers);
	// getters
	std::string getQuestion() const;
	std::vector<std::string> getPossibleAnswers() const;
	int getCorrectAnswer() const;
private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswer;
};

