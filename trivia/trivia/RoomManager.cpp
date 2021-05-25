#include "RoomManager.h"

RoomManager::RoomManager() : _freeID(1)
{
}

bool RoomManager::createRoom(LoggedUser user, RoomData data)
{
	std::lock_guard<std::mutex> usersLock(this->m_roomsMap_lock);
	data.id = this->_freeID++;
	if (this->m_rooms.find(data.id) != this->m_rooms.end())
	{
		return false;
	}
	this->m_rooms[data.id] = Room(data);
	return true;
}

bool RoomManager::deleteRoom(int ID)
{
	std::lock_guard<std::mutex> usersLock(this->m_roomsMap_lock);
	//if exists
	if (this->m_rooms.find(ID) != this->m_rooms.end())
	{
		m_rooms.erase(m_rooms.find(ID));
		return true;
	}
	return false;
}

unsigned int RoomManager::getRoomState(int ID)
{
	//if exists
	if (this->m_rooms.find(ID) != this->m_rooms.end())
	{
		return	m_rooms[ID].getMetaData().isActive;
	}
	throw std::exception("Room wan't found");
}

std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> result;
	for (auto it = m_rooms.begin(); it != m_rooms.end(); it++)
	{  
		result.push_back(it->second.getMetaData());
	}
	return result;
}

Room& RoomManager::getRoom(roomID id)
{
	return this->m_rooms[id];
}

bool RoomManager::doesRoomExist(roomID id)
{
	return this->m_rooms.find(id) != this->m_rooms.end();
}

