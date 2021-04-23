#pragma once

#define PORT 6666

#define REQUEST_CODE_BYTES 1
#define CONTENT_LENGTH_BYTES 4
#define BYTE_SIZE 8

enum
{
	ERROR_CODE = 0,
	LOGIN,
	SIGNUP
};