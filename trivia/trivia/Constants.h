#pragma once

#define PORT 6666

#define REQUEST_CODE_BYTES 1
#define CONTENT_LENGTH_BYTES 4
#define BYTE_SIZE 8
#define DB_PATH "../TriviaDatabase.sqlite"
#define QUESTION_FILE_PATH "../questions.json"

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
	LEAVE_ROOM,
	START_GAME,
	GET_ROOM_STATE
};