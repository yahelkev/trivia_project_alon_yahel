#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(RequestHandlerFactory& factory, LoggedUser user) :
	m_handlerFactory(factory), m_roomManager(factory.getRoomManager()), m_statisticsManager(factory.getStatisticsManager()), m_user(user)
{
}
