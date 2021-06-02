using System.ComponentModel;
using System.Windows;
using System.Windows.Controls;
using System.Threading;
using System.Linq;

namespace Client
{
	public partial class JoinRoomWindow : Window
	{
		private Communicator _communicator;
		private RoomData[] _roomData;
		private RequestWorker _worker = new RequestWorker();
		private BackgroundWorker _refreshWorker = new BackgroundWorker();
		private string _lastSelection;
		private const int REFRESH_TIME = 3000;

		public JoinRoomWindow(Communicator communicator, RoomData[] rooms)
		{
			InitializeComponent();
			_communicator = communicator;
			_roomData = rooms;
			updateRoomList();
			// set up refresh worker
			_refreshWorker.WorkerReportsProgress = true;
			_refreshWorker.DoWork += RefreshWork;
			_refreshWorker.ProgressChanged += RefreshReport;
			_refreshWorker.RunWorkerAsync();
		}

		private uint getRoomId(string roomName)
		{
			foreach (RoomData room in _roomData)
				if (room.name == roomName)
					return room.id;
			return uint.MaxValue;
		}

		private void updateRoomList()
		{
			// set UI
			if (_roomData.Length == 0)
			{   // no available rooms
				RoomList.Visibility = Visibility.Collapsed;
				NoRoomText.Visibility = Visibility.Visible;
				return;
			}
			RoomList.Visibility = Visibility.Visible;
			NoRoomText.Visibility = Visibility.Collapsed;
			// update list
			// order data in arrays
			string[] finalRoomNames = new string[_roomData.Length];
			for (int i = 0; i < _roomData.Length; i++)
				finalRoomNames[i] = _roomData[i].name;
			string[] currentRoomNames = RoomList.Items.OfType<string>().ToArray();
			// get difference in arrays current and final
			string[] toRemove = currentRoomNames.Except(finalRoomNames).ToArray();  // remove elements which are in the list currently but not in the final list
			string[] toAdd = finalRoomNames.Except(currentRoomNames).ToArray(); // add elements which are in the final list but not in the current list

			// change the list
			foreach (string room in toRemove)
				RoomList.Items.Remove(room);
			foreach (string room in toAdd)
				RoomList.Items.Add(room);
		}

		private void Return_Click(object sender, RoutedEventArgs e)
		{
			// return to menu
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}

		private void RefreshWork(object sender, DoWorkEventArgs e)
		{
			while (true)
			{
				Thread.Sleep(REFRESH_TIME);
				// send request and update UI for rooms
				GetRoomsResponse response = _communicator.getRooms();
				_roomData = response.rooms;
				_refreshWorker.ReportProgress(0, null);
			}
		}
		private void RefreshReport(object sender, ProgressChangedEventArgs e)
		{
			updateRoomList();
		}

		private void RoomList_SelectionChanged(object sender, SelectionChangedEventArgs e)
		{
			// get room id
			string roomName = (string)RoomList.SelectedItem;
			uint id = getRoomId(roomName);
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

		// join room request
		private void Join_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(JoinWork, JoinComplete, getRoomId(_lastSelection));
		}
		private void JoinWork(object sender, DoWorkEventArgs e)
		{
			// get rooms request
			JoinRoomResponse response = _communicator.joinRoom((uint)e.Argument);
			e.Result = response.status;
		}
		private void JoinComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			if((uint)e.Result == 0)
			{
				MessageBox.Show("Can't join room", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}
			// change window
			Window window = new RoomWindow(_communicator, getRoomId(_lastSelection), _lastSelection, false);
			Close();
			window.ShowDialog();
		}
	}
}
