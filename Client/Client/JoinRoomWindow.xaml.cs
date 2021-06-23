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
		private RoomData _lastSelection;
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

		private void updateRoomList()
		{
			// set UI
			if (_roomData.Length == 0)
			{   // no available rooms
				RoomList.Visibility = Visibility.Collapsed;
				NoRoomText.Visibility = Visibility.Visible;
				PlayersTitle.Visibility = Visibility.Collapsed;
				PlayerBorder.Visibility = Visibility.Collapsed;
				JoinButton.Visibility = Visibility.Collapsed;
				return;
			}
			RoomList.Visibility = Visibility.Visible;
			NoRoomText.Visibility = Visibility.Collapsed;
			// update list
			uint selectedRoomId = RoomList.SelectedItem == null ? ~0u : ((RoomData)RoomList.SelectedItem).id;
			RoomList.Items.Clear();
			foreach (RoomData room in _roomData)
			{
				RoomList.Items.Add(room);
				if (room.id == selectedRoomId)
					RoomList.SelectedItem = room;
			}
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
			if (RoomList.SelectedItem == null)
				return;
			// get room id
			RoomData room = (RoomData)RoomList.SelectedItem;
			// get player list
			bool canRun = _worker.Run(PlayerListWork, PlayerListComplete, room.id);
			if (canRun) // wasn't already in progress
				_lastSelection = room;
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
			// set list
			PlayersTitle.Visibility = Visibility.Visible;
			PlayerBorder.Visibility = Visibility.Visible;
			JoinButton.Visibility = Visibility.Visible;

			PlayerList.Children.Clear();
			foreach (string player in response.players)
			{
				TextBlock textBlock = new TextBlock();
				textBlock.Text = player;
				PlayerList.Children.Add(textBlock);
			}
			// check if selection was changed again while sending request
			if (_lastSelection.id != ((RoomData)RoomList.SelectedItem).id)
				RoomList_SelectionChanged(null, null);	// update again
		}

		// join room request
		private void Join_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(JoinWork, JoinComplete, _lastSelection.id);
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
			Window window = new RoomWindow(_communicator, _lastSelection.id, _lastSelection.name, false);
			Close();
			window.ShowDialog();
		}
	}
}
