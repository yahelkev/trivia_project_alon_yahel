using System;
using System.Windows;
using System.ComponentModel;

namespace Client
{
	/// <summary>
	/// Interaction logic for CreateRoomWindow.xaml
	/// </summary>
	public partial class CreateRoomWindow : Window
	{
		private Communicator _communicator;
		private BackgroundWorker _worker = new BackgroundWorker();
		public CreateRoomWindow(Communicator communicator)
		{
			InitializeComponent();
			_communicator = communicator;
			_worker.DoWork += CreateWork;
			_worker.RunWorkerCompleted += CreateComplete;
		}

		private void Return_Click(object sender, RoutedEventArgs e)
		{
			// open menu window
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}

		private void Create_Click(object sender, RoutedEventArgs e)
		{
			RoomData roomData = new RoomData();
			bool valid = true;
			// set values
			roomData.name = RoomNameInput.Text;
			if (RoomNameInput.Text == "")
				valid = false;
			uint temp;
			valid &= uint.TryParse(PlayerCountInput.Text, out temp);
			roomData.maxPlayers = temp;
			valid &= uint.TryParse(QuestionCountInput.Text, out roomData.numOfQuestions);
			valid &= uint.TryParse(QuestionTimeInput.Text, out roomData.timePerQuestion);
			// check if valid
			valid &= roomData.maxPlayers > 0;
			valid &= roomData.numOfQuestions > 0;
			valid &= roomData.timePerQuestion > 0;
			if(valid)
				_worker.RunWorkerAsync(roomData);
			else
				MessageBox.Show("Invalid input", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}
		private void CreateWork(object sender, DoWorkEventArgs e)
		{
			RoomData room = (RoomData)e.Argument;
			CreateRoomResponse response = _communicator.createRoom(room.name, room.maxPlayers, room.numOfQuestions, room.timePerQuestion);
			if (response.status == 0)   // not created
				throw new Exception("Can't create room");

			room.id = response.roomId;
			e.Result = room;
		}
		private void CreateComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			if(e.Error != null)
			{
				MessageBox.Show(e.Error.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}
			RoomData room = (RoomData)e.Result;
			// move to waiting room window
			Window window = new RoomWindow(_communicator, room.id, room.name, true);
			Close();
			window.ShowDialog();
		}
	}
}
