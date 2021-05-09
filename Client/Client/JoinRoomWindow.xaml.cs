using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;

namespace Client
{
	public partial class JoinRoomWindow : Window
	{
		private Communicator _communicator;
		private RoomData[] _roomData;
		private RequestWorker _worker = new RequestWorker();
		private string _lastSelection;

		public JoinRoomWindow(Communicator communicator, RoomData[] rooms)
		{
			InitializeComponent();
			_communicator = communicator;
			_roomData = rooms;
			updateRoomList();
		}

		private void updateRoomList()
		{
			RoomList.Items.Clear();
			if (_roomData.Length == 0)
			{   // no available rooms
				RoomList.Visibility = Visibility.Collapsed;
				NoRoomText.Visibility = Visibility.Visible;
				return;
			}
			NoRoomText.Visibility = Visibility.Collapsed;
			foreach (RoomData room in _roomData)
				RoomList.Items.Add(room.name);
		}

		private void Return_Click(object sender, RoutedEventArgs e)
		{
			// return to menu
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}

		private void Refresh_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(RefreshWork, RefreshComplete);
		}
		private void RefreshWork(object sender, DoWorkEventArgs e)
		{
			// get rooms request
			GetRoomsResponse response = _communicator.getRooms();
			e.Result = response;
		}
		private void RefreshComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			GetRoomsResponse response = (GetRoomsResponse)e.Result;
			if(response.status == 0)
			{
				MessageBox.Show("Refresh Failed", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}
			_roomData = response.rooms;
			updateRoomList();
		}

		private void RoomList_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			// get room id
			string roomName = (string)RoomList.SelectedItem;
			uint id = ~0u;
			foreach (RoomData room in _roomData)
				if (room.name == roomName)
					id = room.id;
			// get player list
			bool canRun = _worker.Run(PlayerListWork, PlayerListComplete, id);
			if (canRun) // wasn't already in progress
				_lastSelection = roomName;
		}
		private void PlayerListWork(object sender, DoWorkEventArgs e)
		{
			// get rooms request
			GetPlayersInRoomResponse response = _communicator.getPlayersInRoom((uint)e.Argument);
			e.Result = response;
		}
		private void PlayerListComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			GetPlayersInRoomResponse response = (GetPlayersInRoomResponse)e.Result;
			// set list visible
			PlayersTitle.Visibility = Visibility.Visible;
			PlayerBorder.Visibility = Visibility.Visible;
			JoinButton.Visibility = Visibility.Visible;
			// set list
			PlayerList.Children.Clear();
			foreach (string player in response.players)
			{
				TextBlock textBlock = new TextBlock();
				textBlock.Text = player;
				PlayerList.Children.Add(textBlock);
			}
			// check if selection was changed again while sending request
			if (_lastSelection != (string)RoomList.SelectedItem)
				RoomList_SelectionChanged(null, null);	// update again
		}

		private void Join_Click(object sender, RoutedEventArgs e)
		{
			MessageBox.Show("Not implemented yet");
		}
	}
}
