#pragma once
#include <map>
#include "Room.h"

class RoomManager
{
	std::map<roomID, Room> m_rooms;
	std::mutex m_roomsMap_lock;

	static roomID _freeID;
public:
	//creats a room
	roomID createRoom(LoggedUser, RoomData);
	//deletes a room
	bool deleteRoom(int ID);
	//returns room state (game is on or not)
	unsigned int getRoomState(int ID);
	//returns all the rooms
	std::vector<RoomData> getRooms();
	// return room by id
	Room& getRoom(roomID);
	// check if a room id exists
	bool doesRoomExist(roomID);
};

