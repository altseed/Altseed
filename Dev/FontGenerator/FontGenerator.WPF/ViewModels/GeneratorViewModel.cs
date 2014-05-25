using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FontGenerator.Model;

namespace FontGenerator.WPF.ViewModels
{
	class GeneratorViewModel : INotifyPropertyChanged
	{
		private GenerationConfig config { get; set; }



		public event PropertyChangedEventHandler PropertyChanged;
		public void Raise(string propertyName)
		{
			var ev = PropertyChanged;
			if(ev != null)
			{
				ev(this, new PropertyChangedEventArgs(propertyName));
			}
		}
	}
}
