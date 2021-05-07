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

namespace Client
{
	public partial class LoginWindow : Window
	{
		private Communicator _communicator;
		public LoginWindow()
		{
			_communicator = new Communicator();
		}
		private void login(object sender, RoutedEventArgs e)
        {
			LoginResponse response = _communicator.login(inputUserName.Text, inputPassword.Text);
			if(response.status == 1)
            {
				MenuWindow menu = new MenuWindow(_communicator);
				menu.ShowDialog();
			}
            else
            {
				MessageBox.Show("Incorrect password");
            }
        }
		private void signup(object sender, RoutedEventArgs e)
		{
			signupWindow menu = new signupWindow(_communicator);
			menu.ShowDialog();
		}
	}
}
