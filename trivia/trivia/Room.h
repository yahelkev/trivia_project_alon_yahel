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
	Room() {};
	Room(RoomData roomData) :m_metadata(roomData) {};
	//adds user to room
	bool addUser(LoggedUser);
	//removes user from room
	bool removeUser(LoggedUser);
	//gets all userss names in the room
	std::vector<std::string> getAllUsers();

	void setActive();

	RoomData getMetaData();

	//copies the m_metadata's fields
	Room& operator=(const Room& other);
};

