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
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Windows.Forms;

using System.Reflection;
using FilePackageGenerator.Packaging;

namespace FilePackagingTool.GUI
{
	/// <summary>
	/// MainWindow.xaml の相互作用ロジック
	/// </summary>
	public partial class MainWindow : Window
	{
		public MainWindow()
		{
			InitializeComponent();
		}

		private void btn_export_Click(object sender, RoutedEventArgs e)
		{
			if(!System.IO.Directory.Exists(targetPath))
			{
				System.Windows.Forms.MessageBox.Show("ディレクトリが設定されていません。");
				return;
			}

			var dialog = new SaveFileDialog();

			dialog.Filter = "パックファイル(*.pack)|*.pack";

			if(dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				Packing.Run(targetPath, dialog.FileName, PackagingSetting.IgnoreHiddenAttribute, new string [] {}, txt_password.Text);
				System.Windows.Forms.MessageBox.Show("パッケージを出力しました。");
			}
		}

		private void btn_selectTarget_Click(object sender, RoutedEventArgs e)
		{
			var dialog = new FolderBrowserDialog();

			if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				targetPath = dialog.SelectedPath;
				lbl_target.Content = dialog.SelectedPath;
			}
		}

		string targetPath;
	}
}
