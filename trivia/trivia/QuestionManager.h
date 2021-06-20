#pragma once
#include "IDatabase.h"

class QuestionManager
{
public:
	QuestionManager(IDatabase*);
	void addQuestion(Question);
private:
	IDatabase* _database;
};

