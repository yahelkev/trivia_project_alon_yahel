#pragma once
#include "MessageStructs.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	static Buffer serializeResponse(ErrorResponse);
};

