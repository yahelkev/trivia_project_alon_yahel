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
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.ComponentModel;

namespace Client
{
	public partial class LoginWindow : Window
	{
		private Communicator _communicator;
		private BackgroundWorker _worker = new BackgroundWorker();

		public LoginWindow(Communicator communicator)
		{
			InitializeComponent();
			_communicator = communicator;
			_worker.DoWork += do_work;
			_worker.RunWorkerCompleted += work_complete;
		}
		public LoginWindow()
		{
			InitializeComponent();
			_communicator = new Communicator();
			_worker.DoWork += do_work;
			_worker.RunWorkerCompleted += work_complete;
		}
		private void do_work(object sender, DoWorkEventArgs e)
        {
			LoginResponse response = _communicator.login(((string[])e.Argument)[0], ((string[])e.Argument)[1]);
			e.Result = response.status;
		}
		private void work_complete(object sender, RunWorkerCompletedEventArgs e)
		{
			if (e.Error != null)
			{
				MessageBox.Show(e.Error.Message);
				return;
			}
			if ((uint)e.Result == 1)
			{
				MenuWindow menu = new MenuWindow(_communicator);
				this.Close();
				menu.ShowDialog();
			}
			else
			{
				MessageBox.Show("Incorrect password");
			}
		}
		private void login(object sender, RoutedEventArgs e)
		{
			_worker.RunWorkerAsync(new string[]{inputUserName.Text, inputPassword.Text});
        }
		private void signup(object sender, RoutedEventArgs e)
		{
			signupWindow menu = new signupWindow(_communicator);
			menu.ShowDialog();
		}
	}
}
