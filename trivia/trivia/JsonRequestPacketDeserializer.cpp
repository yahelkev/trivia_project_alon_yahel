#include "JsonRequestPacketDeserializer.h"

int JsonRequestPacketDeserializer::fourByteVecToDecimal(Buffer buffer)
{
	
	int result = 0;
	for (int i = 0; i < CONTENT_LENGTH_BYTES; i++)
	{
		result |= buffer[CONTENT_LENGTH_BYTES - i] << i* BYTE_SIZE;
	}

	return result;
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	LoginRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	return result;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer msg)
{
	json jsonData = json::parse(msg);
	SignupRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	result.email = jsonData["email"];
	return result;
}
