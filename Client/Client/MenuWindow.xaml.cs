﻿using System;
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

		// logout button
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
				SwitchWindow(new LoginWindow(_communicator));
			else
				MessageBox.Show("Logout Failed", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}

		// high scores button
		private void HighScores_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(HighScoresWork, HighScoresComplete);
		}
		private void HighScoresWork(object sender, DoWorkEventArgs e)
		{
			GetHighScoresResponse response = _communicator.getHighScores();
			e.Result = response;
		}
		private void HighScoresComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			GetHighScoresResponse response = (GetHighScoresResponse)e.Result;
			if (response.status == 1)
				OpenWindow(new StatisticsWindow("High Scores", response.statistics));
			else
				MessageBox.Show("Failed to get High Scores", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}

		// user scores button
		private void UserStats_Click(object sender, RoutedEventArgs e)
		{
			_worker.Run(UserStatsWork, UserStatsComplete);
		}
		private void UserStatsWork(object sender, DoWorkEventArgs e)
		{
			GetPersonalStatisticsResponse response = _communicator.getUserStatistics();
			response.status = 1;
			response.statistics = new string[] { "stat1: 5" };
			e.Result = response;
		}
		private void UserStatsComplete(object sender, RunWorkerCompletedEventArgs e)
		{
			GetPersonalStatisticsResponse response = (GetPersonalStatisticsResponse)e.Result;
			if (response.status == 1)
				OpenWindow(new StatisticsWindow("Your Statistics", response.statistics));
			else
				MessageBox.Show("Failed to get your statistics", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
		}

		// function switches this window to another window (keeps this open)
		private void OpenWindow(Window window)
		{
			Visibility = Visibility.Hidden;
			window.ShowDialog();
			Visibility = Visibility.Visible;
		}
		// function switches this window to another window (closes this one)
		private void SwitchWindow(Window window)
		{
			Close();
			window.ShowDialog();
		}
	}
}
