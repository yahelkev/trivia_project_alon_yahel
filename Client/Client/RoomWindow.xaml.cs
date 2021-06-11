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
		private GetRoomStateResponse _roomState;
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
			_refreshWorker.WorkerSupportsCancellation = true;
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
			lock (_communicator)
			{
				_refreshWorker.CancelAsync();
			}
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
			lock (_communicator)
			{
				//stopping the refresh thread
				_refreshWorker.CancelAsync();
				StartGameResponse response = _communicator.startGame();
			}
		}
		private void StartComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			Window window = new GameWindow(_communicator, _id, _roomState.answerTimeout, _roomState.questionCount);
			Close();
			window.ShowDialog();
		}

		private void RefreshThread(object sender, DoWorkEventArgs e)
		{
			while (true)
			{
				// get updated room state
				lock (_communicator)
				{
					if(_refreshWorker.CancellationPending) { return; };
					object response = _communicator.getRoomState();
					_refreshWorker.ReportProgress(0, response);
				}
				Thread.Sleep(REFRESH_TIME);
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
				_roomState = (GetRoomStateResponse)response;
				updatePlayerList(_roomState.players);
			}
			else if(responseType == typeof(LeaveRoomResponse))
			{
				MessageBox.Show("Room Closed...");
				// go to menu
				lock (_communicator)
				{
					_refreshWorker.CancelAsync();
				}
				Window window = new MenuWindow(_communicator);
				Close();
				window.ShowDialog();
			}
			else if(responseType == typeof(StartGameResponse))
			{
				//waits for the refresh thread to finish
				lock (_communicator)
				{
					_refreshWorker.CancelAsync();
				}
				Window window = new GameWindow(_communicator, _id, _roomState.answerTimeout, _roomState.questionCount);
				Close();
				window.ShowDialog();
			}
		}
	}
}
