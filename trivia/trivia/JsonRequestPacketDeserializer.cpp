#include "JsonRequestPacketDeserializer.h"

int JsonRequestPacketDeserializer::fourByteVecToDecimal(Buffer buffer)
{
	std::string bufAsString(buffer.begin() + 1, buffer.begin() + 1 + CONTENT_LENGTH_BYTES);
	int result = std::stoi(bufAsString);
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
