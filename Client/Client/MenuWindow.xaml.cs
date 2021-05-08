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
	public partial class MenuWindow : Window
	{
		private Communicator _communicator;
		private RequestWorker _worker = new RequestWorker();

		public MenuWindow(Communicator communicator)
		{
			InitializeComponent();
			_communicator = communicator;
		}

		private void Logout_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(LogoutWork, LogoutComplete);
		}
		private void LogoutWork(object sender, DoWorkEventArgs e)
		{
			LogoutResponse response = _communicator.logout();
			e.Result = response.status == 1;
		}
		private void LogoutComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			if ((bool)e.Result)
				Close();    // back to login window
			else
				MessageBox.Show("Logout Failed", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}

		// function switches this window to another window
		private void SwitchWindow(Window window)
		{
			Visibility = Visibility.Hidden;
			window.ShowDialog();
			Visibility = Visibility.Visible;
		}
	}
}
