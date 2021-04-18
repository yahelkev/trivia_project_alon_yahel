#pragma once

#define PORT 6666

#define REQUEST_CODE_BYTES 1
#define CONTENT_LENGTH_BYTES 4

enum
{
	ERROR_CODE = 0,
	LOGIN = 1,
	SIGNUP = 2
};