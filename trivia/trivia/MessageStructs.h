#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

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
	roomID roomId;
} CreateRoomResponse;

//--Game
typedef struct
{
	unsigned int status;
} LeaveGameResponse;
// waiting room
typedef struct
{
	unsigned int status;
} CloseRoomResponse;

typedef struct
{
	unsigned int status;
	std::string question;
	std::map<unsigned int, std::string> answers;
} GetQuestionResponse;

typedef struct
{
	unsigned int status;
} LeaveRoomResponse;

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
} StartGameResponse;

typedef struct
{
	unsigned int status;
	std::vector<PlayerResults> results;
} GetGameResultsResponse;

typedef struct
{
	unsigned int status;
	bool hasGameBegun;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
} GetRoomStateResponse;