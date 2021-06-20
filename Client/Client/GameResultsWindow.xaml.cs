using System.Windows;
using System.Windows.Controls;
using System.ComponentModel;
using System.Threading;

namespace Client
{
	public partial class GameResultsWindow : Window
	{
		private Communicator _communicator;
		private BackgroundWorker _menuWorker = new BackgroundWorker();
		private BackgroundWorker _refreshWorker = new BackgroundWorker();
		private const int REFRESH_TIME = 3000;

		public GameResultsWindow(Communicator communicator)
		{
			_communicator = communicator;
			InitializeComponent();
			// setup workers
			_menuWorker.DoWork += MenuWork;
			_menuWorker.RunWorkerCompleted += MenuComplete;
			_refreshWorker.DoWork += GetResultsWork;
			_refreshWorker.RunWorkerCompleted += GetResultsComplete;
			_refreshWorker.RunWorkerAsync();
		}

		private void GetResultsWork(object sender, DoWorkEventArgs e)
		{
			GetGameResultsResponse response = _communicator.getGameResults();
			while (response.status == 0)
			{
				Thread.Sleep(REFRESH_TIME);
				response = _communicator.getGameResults();
			}
			// results available
			e.Result = response.results;
		}
		private void GetResultsComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			PlayerResults[] results = (PlayerResults[])e.Result;
			// display result list
			ResultsPanel.Items.Add(new ResultsRow { place = "", name = "name", correctAnswers = "correct", averageTime = "time" });
			for (int i = 0; i < results.Length; i++)
				ResultsPanel.Items.Add(new ResultsRow
				{
					place = (i + 1).ToString(),
					name = results[i].username,
					correctAnswers = results[i].correctAnswerCount.ToString(),
					averageTime = results[i].averageAnswerTime.ToString("0.00")
				});
			// set list visible
			ResultsPanel.Visibility = Visibility.Visible;
			NotAvailableText.Visibility = Visibility.Collapsed;
		}

		private void Menu_Click(object sender, RoutedEventArgs e)
		{
			_menuWorker.RunWorkerAsync();
		}
		private void MenuWork(object sender, DoWorkEventArgs e)
		{
			_communicator.leaveGame();
		}
		private void MenuComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			Window window = new MenuWindow(_communicator);
			Close();
			window.ShowDialog();
		}

		struct ResultsRow
		{
			public string place { get; set; }
			public string name { get; set; }
			public string correctAnswers { get; set; }
			public string averageTime { get; set; }
		}
	}
}
