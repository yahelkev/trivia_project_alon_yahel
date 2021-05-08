using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
	public class Communicator
	{
		public LoginResponse login(string username, string password)
		{
			return new LoginResponse();
		}
		public SignupResponse signup(string username, string password, string email)
		{
			return new SignupResponse();
		}
		public LogoutResponse logout()
		{
			return new LogoutResponse();
		}
		public JoinRoomResponse joinRoom(uint id)
		{
			return new JoinRoomResponse();
		}
		public GetPlayersInRoomResponse getPlayersInRoom(uint id)
		{
			return new GetPlayersInRoomResponse();
		}
		public CreateRoomResponse createRoom(string name, uint maxUsers, uint questionCount, uint answerTimeout)
		{
			return new CreateRoomResponse();
		}
		public GetPersonalStatisticsResponse getUserStatistics()
		{
			return new GetPersonalStatisticsResponse();
		}
		public GetHighScoresResponse getHighScores()
		{
			return new GetHighScoresResponse();
		}
		public GetRoomsResponse getRooms()
		{
			return new GetRoomsResponse();
		}
	}
}
