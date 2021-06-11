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
		public uint roomId;
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
	public struct LeaveGameResponse
	{
		public uint status;
	}
	public struct GetQuestionResponse
	{
		public uint status;
		public string question;
		public string[] answers;
	}
	public struct SubmitAnswerResponse
	{
		public uint status;
		public uint correctAnswerId;
	}
	public struct GetGameResultsResponse
	{
		public uint status;
		public PlayerResults[] results;
	}

	// data public structs
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
	public struct RoomData
	{
		public string name { get; set; }
		public uint id;
		public uint playerCount { get; set; }
		public uint maxPlayers { get; set; }
		public uint numOfQuestions;
		public uint timePerQuestion;
	}
	public struct PlayerResults
	{
		public string username;
		public uint correctAnswerCount;
		public uint wrongAnswerCount;
		public float averageAnswerTime;
	}

}