#pragma once
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <map>
#include "Question.h"

typedef unsigned int roomID;
typedef unsigned char Byte;
typedef std::vector<Byte> Buffer;

//roomData
typedef struct
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int isActive;
} RoomData;

typedef struct
{
	std::list<Question>::iterator currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	float averageAnswerTime;
	time_t startTime;
	time_t lastQuestionTime;
} GameData;

class IRequestHandler;
// requests
//--Loggin manager
typedef struct
{
	Byte id;
	time_t receivalTime;
	Buffer jsonBuffer;
} RequestInfo;

typedef struct
{
	std::string username;
	std::string password;
} LoginRequest;

typedef struct
{
	std::string username;
	std::string password;
	std::string email;
} SignupRequest;

//--Room manager
typedef struct
{
	unsigned int roomId;
} GetPlayersInRoomRequest;

typedef struct
{
	unsigned int roomId;
} JoinRoomRequest;

typedef struct
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
} CreateRoomRequest;

//--Game
typedef struct
{
	unsigned int answerId;
} SubmitAnswerRequest;


// responses
//--Loggin Manager
typedef struct
{
	Buffer response;
	IRequestHandler* newHandler;
} RequestResult;

typedef struct
{
	unsigned int status;
} LoginResponse;

typedef struct
{
	unsigned int status;
} SignupResponse;

typedef struct
{
	std::string message;
} ErrorResponse;

//--Room Manager
typedef struct
{
	unsigned int status;
} LogoutResponse;

typedef struct
{
	unsigned int status;
	std::vector<RoomData> rooms;
} GetRoomsResponse;

typedef struct
{
	std::vector<std::string> players;
} GetPlayersInRoomResponse;

typedef struct
{
	unsigned int status;
	std::vector<std::string> statistics;
} GetHighScoreResponse;

typedef struct
{
	unsigned int status;
	std::vector<std::string> statistics;
} GetPersonalStatsResponse;

typedef struct
{
	unsigned int status;
} JoinRoomResponse;

typedef struct
{
	unsigned int status;
} CreateRoomResponse;

//--Game
typedef struct
{
	unsigned int status;
} LeaveGameResponse;

typedef struct
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
} GetQuestionResponse;

typedef struct
{
	unsigned int status;
	unsigned int correctAnswerId;
} SubmitAnswerResponse;

typedef struct
{
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
} PlayerResults;

typedef struct
{
	unsigned int status;
	std::vector<PlayerResults> results;
} GetGameResultsResponse;
