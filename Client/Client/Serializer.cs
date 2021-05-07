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
	struct LoginRequest
	{
		public string username;
		public string password;
	}
	struct SignupRequest
	{
		public string username;
		public string password;
		public string email;
	}
	struct GetPlayersInRoomRequest
	{
		public uint roomID;
	}
	struct JoinRoomRequest
	{
		public uint roomID;
	}
	struct CreateRoomRequest
	{
		public string roomName;
		public uint maxUsers;
		public uint questionCount;
		public uint questionTimeout;
	}
}
