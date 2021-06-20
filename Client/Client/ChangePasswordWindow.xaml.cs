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
    /// <summary>
    /// Interaction logic for ChangePasswordWindow.xaml
    /// </summary>
    public partial class ChangePasswordWindow : Window
    {
		private Communicator _communicator;
		private BackgroundWorker _worker = new BackgroundWorker();
		public ChangePasswordWindow(Communicator communicator)
		{
			InitializeComponent();
			_communicator = communicator;
			_worker.DoWork += ChangeWork;
			_worker.RunWorkerCompleted += ChangeComplete;
		}
		private void Change_Click(object sender, RoutedEventArgs e)
		{
			// checks if valid
			if (oldPassInput.Text != "" && newPassInput.Text != "" && confirmInput.Text == newPassInput.Text)
			{
				ChangePasswordRequest request = new ChangePasswordRequest { password = newPassInput.Text, oldPassword = oldPassInput.Text };
				_worker.RunWorkerAsync(request);
			}
			else
				MessageBox.Show("Invalid input - make sure all fields are filled", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}
		private void ChangeWork(object sender, DoWorkEventArgs e)
		{
			ChangePasswordRequest request = (ChangePasswordRequest)e.Argument;
			ChangePasswordResponse response = _communicator.changePassword(request);
			if (response.status == 0)   // not added
				throw new Exception("Can't change password");
		}
		private void ChangeComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			if (e.Error != null)
			{
				MessageBox.Show(e.Error.Message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}
			// move to menu
			returnToMenu();
		}
		private void Return_Click(object sender, RoutedEventArgs e)
		{
			// open menu window
			returnToMenu();
		}
		private void returnToMenu()
		{
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}
	}
}