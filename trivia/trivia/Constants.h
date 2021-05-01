#pragma once

#define PORT 6666

#define REQUEST_CODE_BYTES 1
#define CONTENT_LENGTH_BYTES 4
#define BYTE_SIZE 8
#define DB_PATH "../TriviaDatabase.sqlite"
#define QUESTION_FILE_PATH "../questions.json"
#define HIGHSCORES_USER_COUNT "5"

enum
{
	ERROR_CODE = 0,
	LOGIN,
	SIGNUP
};