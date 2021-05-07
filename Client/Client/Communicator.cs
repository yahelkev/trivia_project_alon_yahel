using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

using System.Net.Sockets;
using System.Net;

namespace Client
{
	public class Communicator
	{
		private const int PORT = 6666;
		private const int MAX_MSG_SIZE = 4096;
		private const int REQUEST_CODE_BYTES = 1;
		private const int CONTENT_LENGTH_BYTES = 4;
		private const string SERVER_IP = "127.0.0.1";
		private NetworkStream _clientStream;
		public Communicator()
		{
			TcpClient client = new TcpClient();
			IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(SERVER_IP), PORT);
			client.Connect(serverEndPoint);
			_clientStream = client.GetStream();
		}

		enum MSG_CODES
		{
			ERROR_CODE = 0,
			LOGIN,
			SIGNUP,
			LOGOUT,
			GET_ROOMS,
			GET_PLAYERS_IN_ROOM,
			JOIN_ROOM,
			CREATE_ROOM,
			HIGH_SCORE,
			USER_STATISTICS
		}
		private Byte[] sendMsg(int code, string msg)
		{
			//sends json and returns json
			//wriets the massg to the server and get the response
			byte[] buffer = new ASCIIEncoding().GetBytes(code.ToString() + msg.Length.ToString() + msg);
			_clientStream.Write(buffer, 0, buffer.Length);
			_clientStream.Flush();
			byte[] msgCode = new byte[REQUEST_CODE_BYTES];
			byte[]  contentLength = new byte[CONTENT_LENGTH_BYTES];
			buffer = new byte[MAX_MSG_SIZE];
			_clientStream.Read(msgCode, 0, REQUEST_CODE_BYTES);
			_clientStream.Read(contentLength, REQUEST_CODE_BYTES, CONTENT_LENGTH_BYTES);
			_clientStream.Read(buffer, REQUEST_CODE_BYTES + REQUEST_CODE_BYTES, MAX_MSG_SIZE);
			return buffer;
		}
		public LoginResponse login(string username, string password)
		{
			LoginRequest msgData = new LoginRequest();
			msgData.username = username;
			msgData.password = password;
			string json = Serializer.serializeRequest<LoginRequest>(msgData).ToString();
			return Deserializer.deserializeResponse<LoginResponse>(sendMsg((int)MSG_CODES.LOGIN, json));
		}
		public SignupResponse signup(string username, string password, string email)
		{
			SignupRequest msgData = new SignupRequest();
			msgData.username = username;
			msgData.password = password;
			msgData.email = email;
			string json = Serializer.serializeRequest<SignupRequest>(msgData).ToString();
			return Deserializer.deserializeResponse<SignupResponse>(sendMsg((int)MSG_CODES.SIGNUP, json));
		}
		public LogoutResponse logout()
		{
			return Deserializer.deserializeResponse<LogoutResponse>(sendMsg((int)MSG_CODES.LOGOUT, ""));
		}
		public JoinRoomResponse joinRoom(uint id)
		{
			JoinRoomRequest msgData = new JoinRoomRequest();
			msgData.roomID = id;
			string json = Serializer.serializeRequest<JoinRoomRequest>(msgData).ToString();
			return Deserializer.deserializeResponse<JoinRoomResponse>(sendMsg((int)MSG_CODES.JOIN_ROOM, json));
		}
		public GetPlayersInRoomResponse getPlayersInRoom(uint id)
		{
			GetPlayersInRoomRequest msgData = new GetPlayersInRoomRequest();
			msgData.roomID = id;
			string json = Serializer.serializeRequest<GetPlayersInRoomRequest>(msgData).ToString();
			return Deserializer.deserializeResponse<GetPlayersInRoomResponse>(sendMsg((int)MSG_CODES.GET_PLAYERS_IN_ROOM, json));
		}
		public CreateRoomResponse createRoom(string name, uint maxUsers, uint questionCount, uint answerTimeout)
		{
			CreateRoomRequest msgData = new CreateRoomRequest();
			msgData.roomName = name;
			msgData.maxUsers = maxUsers;
			msgData.questionCount = questionCount;
			msgData.questionTimeout = answerTimeout;
			string json = Serializer.serializeRequest<CreateRoomRequest>(msgData).ToString();
			return Deserializer.deserializeResponse<CreateRoomResponse>(sendMsg((int)MSG_CODES.CREATE_ROOM, json));
		}
		public GetPersonalStatisticsResponse getUserStatistics()
		{
			return Deserializer.deserializeResponse<GetPersonalStatisticsResponse>(sendMsg((int)MSG_CODES.USER_STATISTICS, ""));
		}
		public GetHighScoresResponse getHighScores()
		{
			return Deserializer.deserializeResponse<GetHighScoresResponse>(sendMsg((int)MSG_CODES.HIGH_SCORE, ""));
		}
		public GetRoomsResponse getRooms()
		{
			return Deserializer.deserializeResponse<GetRoomsResponse>(sendMsg((int)MSG_CODES.GET_ROOMS, ""));
		}
	}
}
