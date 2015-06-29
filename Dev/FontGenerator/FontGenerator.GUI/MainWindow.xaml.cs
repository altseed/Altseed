using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using FontGenerator.WPF.ViewModels;

namespace FontGenerator.WPF
{
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : System.Windows.Window
	{
		public MainWindow()
		{
			InitializeComponent();
			DataContext = new GeneratorViewModel();

			var name2path = Model.Helper.GetFontPairs();

			foreach(var n2p in name2path)
			{
				fontPathes.Items.Add(n2p);
			}
		}

		private async void Button_Click(object sender, RoutedEventArgs e)
		{
			var vm = DataContext as GeneratorViewModel;
			if(vm != null)
			{
				try
				{

					await vm.GenerateAsync();
					MessageBox.Show("フォントの生成に成功しました。", "フォントツール", MessageBoxButton.OK);
				}
				catch (FileNotFoundException error)
				{
					MessageBox.Show(error.Message, "出力エラー", MessageBoxButton.OK);
				}
			}
		}

	}
}
