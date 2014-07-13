using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace FontGenerator.WPF.ViewModels
{
	class DelegateCommand : ICommand
	{
		public event EventHandler CanExecuteChanged;
		public Action<object> CommandHandler { get; set; }
		public Func<object, bool> CanExecuteHandler { get; set; }

		public bool CanExecute(object parameter)
		{
			if(CanExecuteHandler != null)
			{
				return CanExecuteHandler(parameter);
			}
			else
			{
				return true;
			}
		}

		public void Execute(object parameter)
		{
			if(CommandHandler != null)
			{
				CommandHandler(parameter);
			}
		}
	}
}
