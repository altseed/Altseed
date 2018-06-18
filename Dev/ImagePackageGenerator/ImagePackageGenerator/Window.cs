using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ImagePackageGenerator
{
	public partial class Window : Form
	{
		public Window()
		{
			InitializeComponent();
			if (Environment.OSVersion.Platform == PlatformID.Unix)
			{
				// MacのPlatformIDはなぜかUnix
				// TODO: Linux
				Font = new Font( "Hiragino Kaku Gothic Pro", 11 );
				menuStrip.Font = new Font( "Hiragino Kaku Gothic Pro", 11 );
			}
		}

		WindowContext context = null;

		void LoadFile(string path)
		{
			lst_layers.Items.Clear();
			pic.Image = null;
			toolStripStatus.Text = string.Empty;

			context.Load(path);
			
			foreach(var layer in context.Layers)
			{
				lst_layers.Items.Add(layer);
			}
		}

		private void Window_Load(object sender, EventArgs e)
		{
			context = new WindowContext();
		}

		private void Window_FormClosed(object sender, FormClosedEventArgs e)
		{
			context.Terminate();
		}

		private void lst_layers_SelectedValueChanged(object sender, EventArgs e)
		{
			var layer = lst_layers.SelectedItem as WindowContext.Layer;
			pic.Image = layer.Image;
			toolStripStatus.Text = layer.Rect.ToString();
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog ofd = new OpenFileDialog();

			ofd.FileName = "";
			ofd.Filter = "PSD (.psd)|*.psd";
			ofd.Multiselect = false;
			ofd.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

			if(ofd.ShowDialog()== System.Windows.Forms.DialogResult.OK)
			{
				LoadFile(ofd.FileName);
			}
		}

		private void exportToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SaveFileDialog sfd = new SaveFileDialog();

			sfd.FileName = "";
			sfd.Filter = "aip (.aip)|*.aip";
			
			sfd.InitialDirectory = System.IO.Directory.GetCurrentDirectory();

			if(sfd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				System.IO.File.WriteAllBytes(sfd.FileName, context.Export());
			}
		}
	}
}
