#pragma once
#include <string>
#include <vector>
#include <iostream>

typedef unsigned char Byte;
typedef std::vector<Byte> Buffer;

class IRequestHandler;
// requests
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

// responses
typedef struct
{
	Buffer response;
	IRequestHandler* newHandler;
} RequestResult;

typedef struct
{
	bool status;
} LoginResponse;

typedef struct
{
	bool status;
} SignupResponse;

typedef struct
{
	std::string message;
} ERROR_CODEResponse;