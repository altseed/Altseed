using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Threading.Tasks;
using System.Windows;
using FontGenerator.Model;

namespace FontGenerator.WPF
{
	/// <summary>
	/// App.xaml の相互作用ロジック
	/// </summary>
	public partial class App : Application
	{
		private async void Application_Startup(object sender, StartupEventArgs e)
		{
			if(e.Args.Length < 1)
			{
				return;
			}

			var fileName = e.Args[0];
			if(File.Exists(fileName))
			{
				try
				{
					var config = ConfigurationFile.Load(fileName);
					await Generator.GenerateAsync(config);
					Console.WriteLine("出力成功");
					Shutdown();
				}
				catch (SerializationException)
				{
					Console.WriteLine("指定されたファイルは設定ファイルではないか、壊れています。");
				}
				catch(FileNotFoundException err)
				{
					Console.WriteLine(err.Message);
				}
			}
		}
	}
}
