using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.ComponentModel;

namespace Client
{
	public partial class RoomWindow : Window
	{
		private Communicator _communicator;
		private RequestWorker _worker = new RequestWorker();
		private uint _id;
		public RoomWindow(Communicator communicator, uint roomId, string roomName, bool isAdmin)
		{
			InitializeComponent();
			_communicator = communicator;
			_id = roomId;
			RoomName.Text = "Room: \"" + roomName + "\"";
			Refresh();
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

		private void Refresh_Click(object sender, RoutedEventArgs e)
		{
			Refresh();
		}
		private void Refresh()
		{
			_worker.Run(RefreshWork, RefreshComplete);
		}
		private void RefreshWork(object sender, DoWorkEventArgs e)
		{
			// get players list
			GetPlayersInRoomResponse response = _communicator.getPlayersInRoom(_id);
			e.Result = response;
		}
		private void RefreshComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			GetPlayersInRoomResponse response = (GetPlayersInRoomResponse)e.Result;
			updatePlayerList(response.players);
		}
	}
}
