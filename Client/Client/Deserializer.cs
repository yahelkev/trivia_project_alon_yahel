using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
	static class Deserializer
	{
		public static Response deserializeResponse<Response>(byte[] buffer)
		{
			string responseString = new ASCIIEncoding().GetString(buffer);
			return JsonConvert.DeserializeObject<Response>(responseString);
		}
	}
	struct LoginResponse
	{
		public uint status;
	}
	struct SignupResponse
	{
		public uint status;
	}
	struct LogoutResponse
	{
		public uint status;
	}
	struct JoinRoomResponse
	{
		public uint status;
	}
	struct CreateRoomResponse
	{
		public uint status;
	}
	struct GetRoomsResponse
	{
		public uint status;
		public RoomData[] rooms;
	}
	struct GetPlayersInRoomResponse
	{
		public string[] players;
	}
	struct GetHighScoresResponse
	{
		public uint status;
		public string[] statistics;
	}
	struct GetPersonalStatistics
	{
		public uint status;
		public string[] statistics;
	}
	struct ErrorResponse
	{
		public string message;
	}
	
	// data structs
	struct RoomData
	{
		public string name;
		public uint id;
		public uint maxPlayers;
		public uint numOfQuestions;
		public uint timePerQuestion;
		public uint isActive;
	}
}
