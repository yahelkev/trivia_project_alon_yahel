using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace Client
{
	// class wraps BackgroundWorker class to make it work with different methods
	public class RequestWorker
	{
		DoWorkEventHandler _currentDoWork;
		RunWorkerCompletedEventHandler _currentRunWorkerCompleted;
		BackgroundWorker _worker = new BackgroundWorker();

		public RequestWorker()
		{
			_worker.DoWork += DoWork;
			_worker.RunWorkerCompleted += RunWorkerCompleted;
		}

		// function activates the worker with the specified methods if it isn't busy. returns whether it started the request or ignored it.
		public bool Run(DoWorkEventHandler doWork, RunWorkerCompletedEventHandler runWorkerCompleted, object argument = null)
		{
			if (_worker.IsBusy)
				return false;
			// start request
			_currentDoWork = doWork;
			_currentRunWorkerCompleted = runWorkerCompleted;
			_worker.RunWorkerAsync(argument);
			return true;
		}
		// background worker methods
		private void DoWork(object sender, DoWorkEventArgs e)
		{
			_currentDoWork(sender, e);
		}
		private void RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			_currentRunWorkerCompleted(sender, e);
		}
	}
}
