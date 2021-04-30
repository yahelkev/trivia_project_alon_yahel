#pragma once
#include "MessageStructs.h"
#include "Constants.h"
#include "json.hpp"

using json = nlohmann::json;

class JsonRequestPacketDeserializer
{
public:
	static int fourByteVecToDecimal(Buffer vec);
	//deserialize a login request
	static LoginRequest deserializeLoginRequest(Buffer);
	//deserialize a singup request
	static SignupRequest deserializeSignupRequest(Buffer);

	//2.0.0
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(Buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(Buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(Buffer);

};

