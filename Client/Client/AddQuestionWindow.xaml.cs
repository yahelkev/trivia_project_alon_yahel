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
    /// Interaction logic for AddQuestion.xaml
    /// </summary>
    public partial class AddQuestionWindow : Window
    {
        private Communicator _communicator;
        private BackgroundWorker _worker = new BackgroundWorker();
        public AddQuestionWindow(Communicator communicator)
        {
            InitializeComponent();
            _communicator = communicator;
            _worker.DoWork += AddWork;
            _worker.RunWorkerCompleted += AddComplete;
        }
		private void Add_Click(object sender, RoutedEventArgs e)
		{
			bool valid = true;
			uint correctAnswer = 0;
			// checks if valid
			if (AnswerZeroInput.Text == "" || AnswerOneInput.Text == "" || AnswerTwoInput.Text == "" || AnswerThreeInput.Text == "" || QuestionInput.Text == "")
				valid = false;
			if (uint.TryParse(CorrectAnswerIndexInput.Text, out correctAnswer))
				valid &= (correctAnswer <= 3 && correctAnswer >= 0);
			else
				valid = false;

			AddQuestionRequest addRequest = new AddQuestionRequest();
			if (valid)
			{
				addRequest.question = QuestionInput.Text;
				addRequest.answers = new string[] { AnswerZeroInput.Text, AnswerOneInput.Text, AnswerTwoInput.Text, AnswerThreeInput.Text };
				addRequest.correctAnswerIndex = correctAnswer;
				_worker.RunWorkerAsync(addRequest);
			}
			else
				MessageBox.Show("Invalid input - make sure all fields are filled and the index is a number", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}
		private void AddWork(object sender, DoWorkEventArgs e)
		{
			AddQuestionRequest request = (AddQuestionRequest)e.Argument;
			AddQuestionResponse response = _communicator.addQuestion(request);
			if (response.status == 0)   // not added
				throw new Exception("Can't add question");
		}
		private void AddComplete(object sender, RunWorkerCompletedEventArgs e)
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
