using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;
using System.ComponentModel;
using System.Threading;

using System.Windows.Media.Animation;

namespace Client
{
	public partial class GameWindow : Window
	{
		private Communicator _communicator;
		private uint _id;
		private uint _timeLeft;
		private uint _timePerQuestion;
		private uint _questionCount;
		private uint _currentQuestion = 0;
		private bool _gameEnded = false;
		private RequestWorker _worker = new RequestWorker();
		private DispatcherTimer _timer;
		private Button[] _answers;

		private const uint COLOR_ANIMATION_DURATION = 200;
		private const int TIME_BETWEEN_QUESTIONS = 2000;
		private Color LIGHT_RED = Color.FromRgb(255, 150, 150);

		public GameWindow(Communicator communicator, uint id, uint timePerQuestion, uint questionCount)
		{
			_communicator = communicator;
			_id = id;
			_timePerQuestion = timePerQuestion;
			_questionCount = questionCount;
			InitializeComponent();
			_answers = new Button[] { Answer0, Answer1, Answer2, Answer3 };
			// unfreeze animated values
			foreach(Button answer in _answers)
				answer.Background = answer.Background.Clone();
			Background = Background.Clone();
			// start game
			_worker.Run(NextQuestionWork, NextQuestionComplete);
		}

		private void Answer_Click(object sender, RoutedEventArgs e)
		{
			uint answer = uint.Parse((string)((Button)sender).Tag);
			_worker.Run(SubmitAnswerWork, SubmitAnswerComplete, SubmitAnswerReport, answer);
			_timer.Stop();
		}
		private void SubmitAnswerWork(object sender, DoWorkEventArgs e)
		{
			uint answer = (uint)e.Argument;
			SubmitAnswerResponse response = _communicator.submitAnswer((uint)e.Argument);
			// show result
			_worker.ReportProgress(new uint[] { answer, response.correctAnswerId });
			// next question
			Thread.Sleep(TIME_BETWEEN_QUESTIONS);
			if (_currentQuestion >= _questionCount)
			{   // finished game, move to game results window
				_gameEnded = true;
				return;
			}
			NextQuestionWork(sender, e);
		}
		private void SubmitAnswerReport(object sender, ProgressChangedEventArgs e)
		{
			uint[] result = (uint[])e.UserState;
			uint answer = result[0], correctAnswer = result[1];
			bool isCorrect = answer == correctAnswer;
			// set answer colors
			ColorAnimation animation;
			for (int i = 0; i < _answers.Length; i++)
			{
				// create and set animation
				animation = new ColorAnimation(i == correctAnswer ? Colors.Green : Colors.Red, 
					TimeSpan.FromMilliseconds(COLOR_ANIMATION_DURATION));
				_answers[i].Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
			}
			animation = new ColorAnimation(answer == correctAnswer ? Colors.LightGreen : LIGHT_RED,
					TimeSpan.FromMilliseconds(COLOR_ANIMATION_DURATION));
			Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
		}
		private void SubmitAnswerComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			if (_gameEnded)
			{
				Window window = new GameResultsWindow(_communicator);
				Close();
				window.ShowDialog();
				return;
			}
			// reset colors
			ColorAnimation animation;
			for (int i = 0; i < _answers.Length; i++)
			{
				// create and set animation
				animation = new ColorAnimation(Colors.LightGray, TimeSpan.FromMilliseconds(COLOR_ANIMATION_DURATION));
				_answers[i].Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
			}
			animation = new ColorAnimation(Colors.White, TimeSpan.FromMilliseconds(COLOR_ANIMATION_DURATION));
			Background.BeginAnimation(SolidColorBrush.ColorProperty, animation);
			// set new question
			NextQuestionComplete(sender, e);
		}

		private void StartTimer()
		{
			if(_timer != null)
				_timer.Stop();
			_timer = new DispatcherTimer();
			_timer.Interval = TimeSpan.FromSeconds(1);
			_timer.Tick += UpdateTimer;
			_timer.Start();
		}
		private void UpdateTimer(object sender, EventArgs e)
		{
			if(_timeLeft == 0)  // out of time
			{
				_worker.Run(SubmitAnswerWork, SubmitAnswerComplete, SubmitAnswerReport, ~0u);
				_timer.Stop();
				return;
			}
			_timeLeft--;
			TimeProgressBar.Value = 100d * _timeLeft / _timePerQuestion;
			TimeText.Text = _timeLeft.ToString();
		}

		// function gets question from server and resets UI for it
		private void NextQuestionWork(object sender, DoWorkEventArgs e)
		{
			// get question from server
			GetQuestionResponse response = _communicator.getQuestion();
			e.Result = response;
		}
		private void NextQuestionComplete(object sender, RunWorkerCompletedEventArgs e)
		{
				GetQuestionResponse response = (GetQuestionResponse)e.Result;
			// set question UI
			Question.Text = response.question;
			for(int i = 0; i < _answers.Length; i++)
			{
				_answers[i].Content = response.answers[i];
			}
			// question counter
			_currentQuestion++;
	
			QuestionCount.Text = _currentQuestion + " / " + _questionCount;
			// set timer's fields and UI
			_timeLeft = _timePerQuestion;
			TimeProgressBar.Value = 100d;
			TimeText.Text = _timeLeft.ToString();
			StartTimer();
		}

		private void LeaveGame_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(LeaveGameWork, LeaveGameComplete);
		}
		private void LeaveGameWork(object sender, DoWorkEventArgs e)
		{
			_communicator.leaveGame();
		}
		private void LeaveGameComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}
	}
}
