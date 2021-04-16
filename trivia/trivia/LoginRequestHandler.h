#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo) { return true; }
	virtual RequestResult handleRequest(RequestInfo) { return { Buffer(), nullptr }; }
};
