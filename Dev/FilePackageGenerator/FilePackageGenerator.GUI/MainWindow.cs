using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Reflection;
using FilePackageGenerator.Packaging;

namespace FilePackageGenerator.GUI
{
	public partial class MainWindow : Form
	{
		public MainWindow()
		{
			InitializeComponent();
		}

		private void btn_ref_Click(object sender, EventArgs e)
		{
			var dialog = new FolderBrowserDialog();

			if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				targetPath = dialog.SelectedPath;
				lbl_target.Text = dialog.SelectedPath;
			}
		}

		private void btn_export_Click(object sender, EventArgs e)
		{
			if (!System.IO.Directory.Exists(targetPath))
			{
				System.Windows.Forms.MessageBox.Show("ディレクトリが設定されていません。");
				return;
			}

			var dialog = new SaveFileDialog();

			dialog.Filter = "パックファイル(*.pack)|*.pack";

			if (dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				Packing.Run(targetPath, dialog.FileName, PackagingSetting.IgnoreHiddenAttribute, new string[] { }, txt_password.Text);
				System.Windows.Forms.MessageBox.Show("パッケージを出力しました。");
			}
		}

		string targetPath;
	}
}
