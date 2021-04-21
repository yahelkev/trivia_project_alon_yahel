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

json JsonRequestPacketDeserializer::getJson(Buffer msg)
{
	int dataSize = fourByteVecToDecimal(msg);
	return json::parse(msg.begin() + CONTENT_LENGTH_BYTES + 1, msg.begin() + CONTENT_LENGTH_BYTES + 1 + dataSize);
}

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer msg)
{
	json jsonData = getJson(msg);
	LoginRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	return result;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer msg)
{
	json jsonData = getJson(msg);
	SignupRequest result;
	result.password = jsonData["password"];
	result.username = jsonData["username"];
	result.email = jsonData["email"];
	return result;
}
