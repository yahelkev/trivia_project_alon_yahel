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

namespace Client
{
    public partial class signupWindow : Window
    {
        private Communicator _communicator;
        public signupWindow(Communicator communicator)
        {
            InitializeComponent();
            _communicator = communicator;
        }
        private void signup(object sender, RoutedEventArgs e)
        {
            SignupResponse response = _communicator.signup(inputUserName.Text, inputPassword.Text, inputemail.Text);
            if (response.status == 0)
            {
                MessageBox.Show("Something went wrong");
            }
            this.Close();
        }
        private void cancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
