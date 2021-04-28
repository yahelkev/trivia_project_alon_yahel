#pragma once
#include "IRequestHandler.h"
class MenuRequestHandler : public IRequestHandler
{
public:
	virtual bool isRequestRelevant(RequestInfo) { return true; }
	virtual RequestResult handleRequest(RequestInfo) { return RequestResult(); }
};

