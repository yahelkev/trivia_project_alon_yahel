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
    public partial class signupWindow : Window
    {
        private Communicator _communicator;
        private BackgroundWorker _worker = new BackgroundWorker();
        public signupWindow(Communicator communicator)
        {
            InitializeComponent();
            _communicator = communicator;
            _worker.DoWork += do_work;
            _worker.RunWorkerCompleted += work_complete;
        }
        private void do_work(object sender, DoWorkEventArgs e)
        {
            SignupResponse response = _communicator.signup(((string[])e.Argument)[0], ((string[])e.Argument)[1], ((string[])e.Argument)[2]);
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
                this.Close();
            }
            else
            {
                MessageBox.Show("invalid input!");
            }
        }
        private void signup(object sender, RoutedEventArgs e)
        {
            _worker.RunWorkerAsync(new string[] { inputUserName.Text, inputPassword.Text, inputEmail.Text });
        }
        private void cancel(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
