#include "LoginRequestHandler.h"

bool LoginRequestHandler::isRequestRelevant(RequestInfo rqstInfo)
{
	return rqstInfo.id == 1 || rqstInfo.id == 2;
}
