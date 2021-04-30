#include "Question.h"

Question::Question(int argc, char** argv, char** cols) : m_possibleAnswers{"", "", "", ""}, m_correctAnswer(0)
{
	// go over cols
	for (int i = 0; i < argc; i++)
	{
		std::string column(cols[i]);
		// check if column is an answer
		if (column.rfind("answer", 0) != std::string::npos)
		{
			// get answer index
			int answerIndex = column[strlen("answer")] - '0';	// char after "answer"
			// add answer
			this->m_possibleAnswers[answerIndex] = argv[i];
		}
		else if (column == "question")
		{
			this->m_question = argv[i];
		}
		else if (column == "correct_answer")
		{
			this->m_correctAnswer = std::atoi(argv[i]);
		}
	}
}
