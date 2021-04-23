#pragma once
#include "IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo);
	virtual RequestResult handleRequest(RequestInfo);
};
