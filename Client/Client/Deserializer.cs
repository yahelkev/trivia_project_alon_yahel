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
	public struct LoginResponse
	{
		public uint status;
	}
	public struct SignupResponse
	{
		public uint status;
	}
	public struct LogoutResponse
	{
		public uint status;
	}
	public struct JoinRoomResponse
	{
		public uint status;
	}
	public struct CreateRoomResponse
	{
		public uint status;
	}
	public struct GetRoomsResponse
	{
		public uint status;
		public RoomData[] rooms;
	}
	public struct GetPlayersInRoomResponse
	{
		public string[] players;
	}
	public struct GetHighScoresResponse
	{
		public uint status;
		public string[] HighScores;
	}
	public struct GetPersonalStatisticsResponse
	{
		public uint status;
		public string[] UserStatistics;
	}
	public struct ErrorResponse
	{
		public string message;
	}
	public struct LeaveRoomResponse
	{
		public uint status;
	}
	public struct StartGameResponse
	{
		public uint status;
	}
	public struct GetRoomStateResponse
	{
		public uint status;
		public bool hasGameBegun;
		public string[] players;
		public uint questionCount;
		public uint answerTimeout;
	}
// data public structs
public struct RoomData
	{
		public string name;
		public uint id;
		public uint maxPlayers;
		public uint numOfQuestions;
		public uint timePerQuestion;
	}
}
