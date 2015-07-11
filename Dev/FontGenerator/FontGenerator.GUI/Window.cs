using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FontGenerator.GUI
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
			}
		}

		ViewModels.GeneratorViewModel viewModel = null;

		bool loading = false;

		void LoadConfig()
		{
			loading = true;

			cb_font.SelectedIndex = viewModel.FontIndex;

			txt_txt.Text = viewModel.TextPath;
			txt_export.Text = viewModel.ExportPath;
			txt_sheet.Text = viewModel.SheetName;

			txt_fontsize.Text = viewModel.FontSize.ToString();
			txt_textureSize.Text = viewModel.TextureSize.ToString();
			txt_outline.Text = viewModel.OutlineSize.ToString();

			tb_color_r.Value = viewModel.Red;
			tb_color_g.Value = viewModel.Green;
			tb_color_b.Value = viewModel.Blue;

			tb_outlineColor_r.Value = viewModel.OutlineRed;
			tb_outlineColor_g.Value = viewModel.OutlineGreen;
			tb_outlineColor_b.Value = viewModel.OutlineBlue;

			cb_font.SelectedIndex = viewModel.FontIndex;
			loading = false;
		}

		private void Window_Load(object sender, EventArgs e)
		{
			viewModel = new ViewModels.GeneratorViewModel();

			var name2path = Model.Helper.GetFontPairs();

			foreach (var n2p in name2path)
			{
				cb_font.Items.Add(n2p);
			}

			LoadConfig();

			timer.Start();
		}

		private void btn_txt_Click(object sender, EventArgs e)
		{
			viewModel.OpenTextFileCommand.Execute(null);
			txt_txt.Text = viewModel.TextPath;
		}

		private void btn_export_Click(object sender, EventArgs e)
		{
			viewModel.OpenExportPathCommand.Execute(null);
			txt_export.Text = viewModel.ExportPath;
		}

		private async void btn_generate_Click(object sender, EventArgs e)
		{
			try
			{
				await viewModel.GenerateAsync();
				MessageBox.Show("フォントの生成に成功しました。", "フォントツール", MessageBoxButtons.OK);
			}
			catch (Exception error)
			{
				MessageBox.Show(error.Message, "出力エラー", MessageBoxButtons.OK);
			}
		}

		private void btn_saveSetting_Click(object sender, EventArgs e)
		{
			viewModel.SaveConfigurationCommand.Execute(null);
		}

		private void btn_loadSetting_Click(object sender, EventArgs e)
		{
			viewModel.LoadConfigurationCommand.Execute(null);

			LoadConfig();
		}

		private void txt_sheet_TextChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.SheetName = txt_sheet.Text;
		}

		private void txt_fontsize_TextChanged(object sender, EventArgs e)
		{
			if (loading) return;

			int value;
			if(int.TryParse(txt_fontsize.Text, out value))
			{
				viewModel.FontSize = value;
			}
		}

		private void txt_textureSize_TextChanged(object sender, EventArgs e)
		{
			if (loading) return;

			int value;
			if (int.TryParse(txt_textureSize.Text, out value))
			{
				viewModel.TextureSize = value;
			}
		}

		private void txt_outline_TextChanged(object sender, EventArgs e)
		{
			if (loading) return;

			int value;
			if (int.TryParse(txt_outline.Text, out value))
			{
				viewModel.OutlineSize = value;
			}
		}

		private void tb_color_r_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.Red = (byte)tb_color_r.Value;	
		}

		private void tb_color_g_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.Green = (byte)tb_color_g.Value;	
		}

		private void tb_color_b_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.Blue = (byte)tb_color_b.Value;	
		}

		private void tb_outlineColor_r_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.OutlineRed = (byte)tb_outlineColor_r.Value;	
		}

		private void tb_outlineColor_g_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.OutlineGreen = (byte)tb_outlineColor_g.Value;	
		}

		private void tb_outlineColor_b_ValueChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.OutlineBlue = (byte)tb_outlineColor_b.Value;	
		}

		private void timer_Tick(object sender, EventArgs e)
		{
			if(pic_preview.Image != viewModel.PreviewImage)
			{
				pic_preview.Image = viewModel.PreviewImage;
			}	
		}

		private void cb_font_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (loading) return;

			viewModel.FontIndex = cb_font.SelectedIndex;
		}

		private void Window_FormClosed(object sender, FormClosedEventArgs e)
		{
			viewModel.Terminate();
		}
	}
}
