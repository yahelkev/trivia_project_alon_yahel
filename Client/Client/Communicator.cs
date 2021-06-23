using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.IO;

using System.Net.Sockets;
using System.Net;

namespace Client
{
	public class Communicator
	{
		private const int REQUEST_CODE_BYTES = 1;
		private const int CONTENT_LENGTH_BYTES = 4;
		private NetworkStream _clientStream;
		private const string CONFIG_PATH = "../../../config.txt";
		public Communicator()
		{
			// get config values
			Dictionary<string, string> config = GetConfig();
			// open connection
			TcpClient client = new TcpClient();
			IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(config["ip"]), int.Parse(config["port"]));
			client.Connect(serverEndPoint);
			_clientStream = client.GetStream();
		}
		
		private Dictionary<string, string> GetConfig()
		{
			Dictionary<string, string> configDictionary = new Dictionary<string, string>();
			// read config file
			using (StreamReader stream = new StreamReader(CONFIG_PATH))
			{
				string line;
				while((line = stream.ReadLine()) != null)
				{
					string[] splitString = line.Split('=');
					if(splitString.Length == 2)  // has name and value
					{
						configDictionary[splitString[0]] = splitString[1];
					}
				}
			}
			return configDictionary;
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
			GAME_RESULTS,
			SUBMIT_ANSWER,
			GET_QUESTION,
			LEAVE_GAME,
			LEAVE_ROOM,
			START_GAME,
			GET_ROOM_STATE,
			RESET_PASSWORD,
			CHANGE_PASSWORD,
			ADD_QUESTION
		}
		public struct ResponseStruct
        {
			public byte[] data;
			public byte code;
		}
		private byte[] sendMsg(int code, byte[] msg)
		{
			return sendMsgGetCode(code, msg).data;
		}
		private ResponseStruct sendMsgGetCode(int code, byte[] msg)
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
			if(msgCode[0] == (int)MSG_CODES.ERROR_CODE)
            {
				throw new Exception(Deserializer.deserializeResponse<ErrorResponse>(buffer).message);
			}
			return new ResponseStruct{data = buffer, code = msgCode[0] };
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
		public GetGameResultsResponse getGameResults()
		{
			return Deserializer.deserializeResponse<GetGameResultsResponse>(sendMsg((int)MSG_CODES.GAME_RESULTS, new byte[] { }));
		}
		public SubmitAnswerResponse submitAnswer(uint answer)
		{
			SubmitAnswerRequest msgData = new SubmitAnswerRequest { answerId = answer };
			byte[] json = Serializer.serializeRequest(msgData);
			return Deserializer.deserializeResponse<SubmitAnswerResponse>(sendMsg((int)MSG_CODES.SUBMIT_ANSWER, json));
		}
		public GetQuestionResponse getQuestion()
		{ 
			return Deserializer.deserializeResponse<GetQuestionResponse>(sendMsg((int)MSG_CODES.GET_QUESTION, new byte[] { }));
		}
		public LeaveGameResponse leaveGame()
		{
			return Deserializer.deserializeResponse<LeaveGameResponse>(sendMsg((int)MSG_CODES.LEAVE_GAME, new byte[] { }));
		}
		public LeaveRoomResponse leaveRoom()
		{
			return Deserializer.deserializeResponse<LeaveRoomResponse>(sendMsg((int)MSG_CODES.LEAVE_ROOM, new byte[] { }));
		}
		public StartGameResponse startGame()
		{
			return Deserializer.deserializeResponse<StartGameResponse>(sendMsg((int)MSG_CODES.START_GAME, new byte[] { }));
		}
		public ResetPasswordResponse resetPassword(string userName)
		{
			ResetPasswordRequest msgData = new ResetPasswordRequest { userName = userName };
			byte[] json = Serializer.serializeRequest(msgData);
			return Deserializer.deserializeResponse<ResetPasswordResponse>(sendMsg((int)MSG_CODES.RESET_PASSWORD,json));
		}
		public ChangePasswordResponse changePassword(ChangePasswordRequest msgData)
		{
			byte[] json = Serializer.serializeRequest(msgData);
			return Deserializer.deserializeResponse<ChangePasswordResponse>(sendMsg((int)MSG_CODES.CHANGE_PASSWORD, json));
		}
		public AddQuestionResponse addQuestion(AddQuestionRequest requestData)
		{
			byte[] json = Serializer.serializeRequest<AddQuestionRequest>(requestData);
			return Deserializer.deserializeResponse<AddQuestionResponse>(sendMsg((int)MSG_CODES.ADD_QUESTION, json));
		}
		public object getRoomState()
		{
			ResponseStruct response = sendMsgGetCode((int)MSG_CODES.GET_ROOM_STATE, new byte[] { });
			switch(response.code)
			{ 	
			case (byte)MSG_CODES.LEAVE_ROOM:
				return Deserializer.deserializeResponse<LeaveRoomResponse>(response.data);
			case (byte)MSG_CODES.START_GAME:
				return Deserializer.deserializeResponse<StartGameResponse>(response.data);
			case (byte)MSG_CODES.GET_ROOM_STATE:
				return Deserializer.deserializeResponse<GetRoomStateResponse>(response.data);
			default:
				throw new Exception("invalid response from server!");

			}
		}
	}
}
