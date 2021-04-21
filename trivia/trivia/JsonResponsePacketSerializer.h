#pragma once
#include "MessageStructs.h"
#include "Constants.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(LoginResponse);
	static Buffer serializeResponse(SignupResponse);
	static Buffer serializeResponse(ErrorResponse);
private:
	// function gets response code and json and returns a buffer that can be sent back to the client
	static Buffer createResponseBuffer(Byte code, json& content);
};

