#pragma once
#include "MessageStructs.h"
#include "LoggedUser.h"

class Room
{
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;

public:
	bool addUser(LoggedUser);
	bool removeUser(LoggedUser);
	std::vector<std::string> getAllUsers();
};

