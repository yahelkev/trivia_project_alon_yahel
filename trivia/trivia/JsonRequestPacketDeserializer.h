#pragma once
#include "MessageStructs.h"
#include "Constants.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
	static int fourByteVecToDecimal(Buffer vec);
	static json getJson(Buffer vec);
public:
	//deserialize a login request
	static LoginRequest deserializeLoginRequest(Buffer);
	//deserialize a singup request
	static SignupRequest deserializeSignupRequest(Buffer);
};

