using System;
using System.Windows;
using System.Windows.Controls;
using System.ComponentModel;
using System.Threading;

namespace Client
{
	public partial class RoomWindow : Window
	{
		private Communicator _communicator;
		private RequestWorker _worker = new RequestWorker();
		private BackgroundWorker _refreshWorker = new BackgroundWorker();
		private uint _id;
		private const int REFRESH_TIME = 3000;
		public RoomWindow(Communicator communicator, uint roomId, string roomName, bool isAdmin)
		{
			InitializeComponent();
			_communicator = communicator;
			_id = roomId;
			RoomName.Text = "Room: \"" + roomName + "\"";
			// setup buttons
			if (isAdmin)
			{
				LeaveButton.Visibility = Visibility.Collapsed;
			}
			else
			{
				CloseButton.Visibility = Visibility.Collapsed;
				StartButton.Visibility = Visibility.Collapsed;
			}
			// setup refresh worker
			_refreshWorker.WorkerReportsProgress = true;
			_refreshWorker.DoWork += RefreshThread;
			_refreshWorker.ProgressChanged += RefreshReport;
			_refreshWorker.RunWorkerAsync();
		}

		private void updatePlayerList(string[] players)
		{
			PlayerList.Children.Clear();
			// add players to list
			foreach (string player in players)
			{
				TextBlock textBlock = new TextBlock();
				textBlock.Text = player;
				PlayerList.Children.Add(textBlock);
			}
		}

		private void Leave_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(LeaveWork, LeaveComplete);
		}
		private void LeaveWork(object sender, DoWorkEventArgs e)
		{
			LeaveRoomResponse response = _communicator.leaveRoom();
		}
		private void LeaveComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			// switch to menu
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}

		private void Start_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(StartWork, StartComplete);
		}
		private void StartWork(object sender, DoWorkEventArgs e)
		{
			StartGameResponse response = _communicator.startGame();
		}
		private void StartComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			// game window not implemented...
		}

		private void RefreshThread(object sender, DoWorkEventArgs e)
		{
			while (true)
			{
				Thread.Sleep(REFRESH_TIME);
				// get updated room state
				object response = _communicator.getRoomState();
				_refreshWorker.ReportProgress(0, response);
			}
		}
		private void RefreshReport(object sender, ProgressChangedEventArgs e)
		{
			object response = e.UserState;
			Type responseType = response.GetType();
			// check possible responses
			if(responseType == typeof(GetRoomStateResponse))
			{
				// update player list
				updatePlayerList(((GetRoomStateResponse)response).players);
			}
			else if(responseType == typeof(LeaveRoomResponse))
			{
				MessageBox.Show("Room Closed...");
				// go to menu
				Window window = new MenuWindow(_communicator);
				Close();
				window.ShowDialog();
			}
			else if(responseType == typeof(StartGameResponse))
			{
				// game window not implemented...
			}
		}
	}
}
