#include "QuestionManager.h"

QuestionManager::QuestionManager(IDatabase* database) : _database(database)
{
}

void QuestionManager::addQuestion(Question question)
{
	this->_database->addQuestion(question);
}
