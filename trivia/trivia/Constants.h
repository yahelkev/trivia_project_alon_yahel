#pragma once

#define PORT 6666

#define REQUEST_CODE_BYTES 1
#define CONTENT_LENGTH_BYTES 4
#define NUM_OF_QUESTIONS 4
#define BYTE_SIZE 8
#define DB_PATH "../TriviaDatabase.sqlite"

#define HIGHSCORES_USER_COUNT "5"
#define HIGHSCORE_GAME_COUNT_EXPONENT 0.1
#define HIGHSCORE_ANSWER_TIME_OFFSET 30

#define INVALID_ROOM 0

enum
{
	ERROR_CODE = 0,
	LOGIN,
	SIGNUP,
	LOGOUT,
	GET_ROOMS,
	GET_PLAYERS_IN_ROOM,
	JOIN_ROOM,
	CREATE_ROOM,
	HIGH_SCORE,
	USER_STATISTICS,
	GAME_RESULTS,
	SUBMIT_ANSWER,
	GET_QUESTION,
	LEAVE_GAME,
	LEAVE_ROOM,
	START_GAME,
	GET_ROOM_STATE
};