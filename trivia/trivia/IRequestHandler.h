#pragma once
#include "MessageStructs.h"
#include "Constants.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

class IRequestHandler
{
public:
	//checks if the code is fitting to the client state
	virtual bool isRequestRelevant(RequestInfo) = 0;
	//proccesses the request and returns a resones and the next state
	virtual RequestResult handleRequest(RequestInfo) = 0;
};

