using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace Client
{
	static class Serializer
	{
		public static byte[] serializeRequest<RequestStruct>(RequestStruct request)
		{
			return new ASCIIEncoding().GetBytes(JsonConvert.SerializeObject(request));
		}
	}
	public struct LoginRequest
	{
		public string username;
		public string password;
	}
	public struct SignupRequest
	{
		public string username;
		public string password;
		public string email;
	}
	public struct GetPlayersInRoomRequest
	{
		public uint roomID;
	}
	public struct JoinRoomRequest
	{
		public uint roomID;
	}
	public struct CreateRoomRequest
	{
		public string roomName;
		public uint maxUsers;
		public uint questionCount;
		public uint answerTimeout;
	}
	public struct SubmitAnswerRequest
	{
		public uint answerId;
	}
}
