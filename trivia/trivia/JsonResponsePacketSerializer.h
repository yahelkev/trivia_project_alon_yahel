#pragma once
#include "MessageStructs.h"

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	static Buffer serializeResponse(ERROR_CODEResponse);
};

