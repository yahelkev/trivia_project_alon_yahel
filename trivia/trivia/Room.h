#pragma once
#include "MessageStructs.h"
#include "LoggedUser.h"
#include <mutex>

class Room
{
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
	std::mutex m_users_lock;

public:
	//adds user to room
	bool addUser(LoggedUser);
	//removes user from room
	bool removeUser(LoggedUser);
	//gets all userss names in the room
	std::vector<std::string> getAllUsers();
};

