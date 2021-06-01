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
			USER_STATISTICS,
			LEAVE_ROOM,
			START_GAME,
			GET_ROOM_STATE
		}
		private byte[] sendMsg(int code, byte[] msg)
		{
			//creates the msg
			byte[] buffer = new byte[REQUEST_CODE_BYTES + CONTENT_LENGTH_BYTES + msg.Length];
			buffer[0] = (byte)code;
			BitConverter.GetBytes(msg.Length).CopyTo(buffer, REQUEST_CODE_BYTES);
			msg.CopyTo(buffer, REQUEST_CODE_BYTES + CONTENT_LENGTH_BYTES);
			//sends to server
			_clientStream.Write(buffer, 0, buffer.Length);
			_clientStream.Flush();
			//gets response (only the json)
			byte[] msgCode = new byte[REQUEST_CODE_BYTES];
			byte[] contentLength = new byte[CONTENT_LENGTH_BYTES];
			_clientStream.Read(msgCode, 0, REQUEST_CODE_BYTES);
			_clientStream.Read(contentLength, 0, CONTENT_LENGTH_BYTES);
			buffer = new byte[BitConverter.ToInt32(contentLength, 0)];
			_clientStream.Read(buffer, 0, BitConverter.ToInt32(contentLength, 0));
			//if somthing whent wrong
			if (msgCode[0] == (int)MSG_CODES.ERROR_CODE)
			{
				throw new Exception(Deserializer.deserializeResponse<ErrorResponse>(buffer).message);
			}
			return buffer;
		}
		public LoginResponse login(string username, string password)
		{
			LoginRequest msgData = new LoginRequest();
			msgData.username = username;
			msgData.password = password;
			byte[] json = Serializer.serializeRequest<LoginRequest>(msgData);
			return Deserializer.deserializeResponse<LoginResponse>(sendMsg((int)MSG_CODES.LOGIN, json));
		}
		public SignupResponse signup(string username, string password, string email)
		{
			SignupRequest msgData = new SignupRequest();
			msgData.username = username;
			msgData.password = password;
			msgData.email = email;
			byte[] json = Serializer.serializeRequest<SignupRequest>(msgData);
			return Deserializer.deserializeResponse<SignupResponse>(sendMsg((int)MSG_CODES.SIGNUP, json));
		}
		public LogoutResponse logout()
		{
			return Deserializer.deserializeResponse<LogoutResponse>(sendMsg((int)MSG_CODES.LOGOUT, new byte[] { }));
		}
		public JoinRoomResponse joinRoom(uint id)
		{
			JoinRoomRequest msgData = new JoinRoomRequest();
			msgData.roomID = id;
			byte[] json = Serializer.serializeRequest<JoinRoomRequest>(msgData);
			return Deserializer.deserializeResponse<JoinRoomResponse>(sendMsg((int)MSG_CODES.JOIN_ROOM, json));
		}
		public GetPlayersInRoomResponse getPlayersInRoom(uint id)
		{
			GetPlayersInRoomRequest msgData = new GetPlayersInRoomRequest();
			msgData.roomID = id;
			byte[] json = Serializer.serializeRequest<GetPlayersInRoomRequest>(msgData);
			return Deserializer.deserializeResponse<GetPlayersInRoomResponse>(sendMsg((int)MSG_CODES.GET_PLAYERS_IN_ROOM, json));
		}
		public CreateRoomResponse createRoom(string name, uint maxUsers, uint questionCount, uint answerTimeout)
		{
			CreateRoomRequest msgData = new CreateRoomRequest();
			msgData.roomName = name;
			msgData.maxUsers = maxUsers;
			msgData.questionCount = questionCount;
			msgData.answerTimeout = answerTimeout;
			byte[] json = Serializer.serializeRequest<CreateRoomRequest>(msgData);
			return Deserializer.deserializeResponse<CreateRoomResponse>(sendMsg((int)MSG_CODES.CREATE_ROOM, json));
		}
		public GetPersonalStatisticsResponse getUserStatistics()
		{
			return Deserializer.deserializeResponse<GetPersonalStatisticsResponse>(sendMsg((int)MSG_CODES.USER_STATISTICS, new byte[] { }));
		}
		public GetHighScoresResponse getHighScores()
		{
			return Deserializer.deserializeResponse<GetHighScoresResponse>(sendMsg((int)MSG_CODES.HIGH_SCORE, new byte[] { }));
		}
		public GetRoomsResponse getRooms()
		{
			return Deserializer.deserializeResponse<GetRoomsResponse>(sendMsg((int)MSG_CODES.GET_ROOMS, new byte[] { }));
		}
		public LeaveRoomResponse leaveRoom()
		{
			return Deserializer.deserializeResponse<LeaveRoomResponse>(sendMsg((int)MSG_CODES.LEAVE_ROOM, new byte[] { }));
		}
		public StartGameResponse startGame()
		{
			return Deserializer.deserializeResponse<StartGameResponse>(sendMsg((int)MSG_CODES.START_GAME, new byte[] { }));
		}
		public object getRoomState()
		{
			byte[] response = sendMsg((int)MSG_CODES.GET_ROOM_STATE, new byte[] { });
			switch(response[0])
			{ 	
			case (byte)MSG_CODES.LEAVE_ROOM:
				return Deserializer.deserializeResponse<LeaveRoomResponse>(response);
			case (byte)MSG_CODES.START_GAME:
				return Deserializer.deserializeResponse<StartGameResponse>(response);
			case (byte)MSG_CODES.GET_ROOM_STATE:
				return Deserializer.deserializeResponse<GetRoomStateResponse>(response);
			default:
				throw new Exception("invalid response from server!");

			}
		}
	}
}
