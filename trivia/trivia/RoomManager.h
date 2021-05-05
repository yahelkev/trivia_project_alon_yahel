#pragma once
#include <map>
#include "Room.h"

class RoomManager
{
	std::map<roomID, Room> m_rooms;
	std::mutex m_roomsMap_lock;


public:
	//creats a room
	bool creatRoom(LoggedUser, RoomData);
	//deletes a room
	bool deleteRoom(int ID);
	//returns room state (game is on or not)
	unsigned int getRoomState(int ID);
	//returns all the rooms
	std::vector<RoomData> getRooms();
};

