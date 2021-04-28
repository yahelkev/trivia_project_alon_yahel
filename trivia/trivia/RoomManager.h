#pragma once
#include <map>
#include "Room.h"

class RoomManager
{
	std::map<roomID, Room> m_rooms;

public:
	bool creatRoom(LoggedUser, RoomData);
	bool deleteRoom(int ID);
	unsigned int getRoomState(int ID);
	std::vector<RoomData> getRooms();
};

