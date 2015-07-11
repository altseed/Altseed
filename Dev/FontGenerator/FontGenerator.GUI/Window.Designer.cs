namespace FontGenerator.GUI
{
	partial class Window
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.grp_font = new System.Windows.Forms.GroupBox();
			this.grp_outlineColor = new System.Windows.Forms.GroupBox();
			this.lbl_outlineColor_b = new System.Windows.Forms.Label();
			this.tb_outlineColor_b = new System.Windows.Forms.TrackBar();
			this.tb_outlineColor_g = new System.Windows.Forms.TrackBar();
			this.lbl_outlineColor_g = new System.Windows.Forms.Label();
			this.lbl_outlineColor_r = new System.Windows.Forms.Label();
			this.tb_outlineColor_r = new System.Windows.Forms.TrackBar();
			this.grp_color = new System.Windows.Forms.GroupBox();
			this.lbl_color_b = new System.Windows.Forms.Label();
			this.tb_color_b = new System.Windows.Forms.TrackBar();
			this.tb_color_g = new System.Windows.Forms.TrackBar();
			this.lbl_color_g = new System.Windows.Forms.Label();
			this.lbl_color_r = new System.Windows.Forms.Label();
			this.tb_color_r = new System.Windows.Forms.TrackBar();
			this.lbl_outline = new System.Windows.Forms.Label();
			this.txt_outline = new System.Windows.Forms.TextBox();
			this.txt_textureSize = new System.Windows.Forms.TextBox();
			this.lbl_textureSize = new System.Windows.Forms.Label();
			this.txt_fontsize = new System.Windows.Forms.TextBox();
			this.lbl_fontsize = new System.Windows.Forms.Label();
			this.txt_sheet = new System.Windows.Forms.TextBox();
			this.lbl_sheet = new System.Windows.Forms.Label();
			this.btn_export = new System.Windows.Forms.Button();
			this.txt_export = new System.Windows.Forms.TextBox();
			this.lbl_export = new System.Windows.Forms.Label();
			this.btn_txt = new System.Windows.Forms.Button();
			this.txt_txt = new System.Windows.Forms.TextBox();
			this.lbl_txt = new System.Windows.Forms.Label();
			this.cb_font = new System.Windows.Forms.ComboBox();
			this.lbl_font = new System.Windows.Forms.Label();
			this.grp_preview = new System.Windows.Forms.GroupBox();
			this.pic_preview = new System.Windows.Forms.PictureBox();
			this.btn_generate = new System.Windows.Forms.Button();
			this.btn_saveSetting = new System.Windows.Forms.Button();
			this.btn_loadSetting = new System.Windows.Forms.Button();
			this.timer = new System.Windows.Forms.Timer(this.components);
			this.grp_font.SuspendLayout();
			this.grp_outlineColor.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_b)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_g)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_r)).BeginInit();
			this.grp_color.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_b)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_g)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_r)).BeginInit();
			this.grp_preview.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pic_preview)).BeginInit();
			this.SuspendLayout();
			// 
			// grp_font
			// 
			this.grp_font.Controls.Add(this.grp_outlineColor);
			this.grp_font.Controls.Add(this.grp_color);
			this.grp_font.Controls.Add(this.lbl_outline);
			this.grp_font.Controls.Add(this.txt_outline);
			this.grp_font.Controls.Add(this.txt_textureSize);
			this.grp_font.Controls.Add(this.lbl_textureSize);
			this.grp_font.Controls.Add(this.txt_fontsize);
			this.grp_font.Controls.Add(this.lbl_fontsize);
			this.grp_font.Controls.Add(this.txt_sheet);
			this.grp_font.Controls.Add(this.lbl_sheet);
			this.grp_font.Controls.Add(this.btn_export);
			this.grp_font.Controls.Add(this.txt_export);
			this.grp_font.Controls.Add(this.lbl_export);
			this.grp_font.Controls.Add(this.btn_txt);
			this.grp_font.Controls.Add(this.txt_txt);
			this.grp_font.Controls.Add(this.lbl_txt);
			this.grp_font.Controls.Add(this.cb_font);
			this.grp_font.Controls.Add(this.lbl_font);
			this.grp_font.Location = new System.Drawing.Point(13, 13);
			this.grp_font.Name = "grp_font";
			this.grp_font.Size = new System.Drawing.Size(599, 344);
			this.grp_font.TabIndex = 0;
			this.grp_font.TabStop = false;
			this.grp_font.Text = "フォント設定";
			// 
			// grp_outlineColor
			// 
			this.grp_outlineColor.Controls.Add(this.lbl_outlineColor_b);
			this.grp_outlineColor.Controls.Add(this.tb_outlineColor_b);
			this.grp_outlineColor.Controls.Add(this.tb_outlineColor_g);
			this.grp_outlineColor.Controls.Add(this.lbl_outlineColor_g);
			this.grp_outlineColor.Controls.Add(this.lbl_outlineColor_r);
			this.grp_outlineColor.Controls.Add(this.tb_outlineColor_r);
			this.grp_outlineColor.Location = new System.Drawing.Point(265, 195);
			this.grp_outlineColor.Name = "grp_outlineColor";
			this.grp_outlineColor.Size = new System.Drawing.Size(250, 140);
			this.grp_outlineColor.TabIndex = 22;
			this.grp_outlineColor.TabStop = false;
			this.grp_outlineColor.Text = "枠線色";
			// 
			// lbl_outlineColor_b
			// 
			this.lbl_outlineColor_b.AutoSize = true;
			this.lbl_outlineColor_b.Location = new System.Drawing.Point(16, 84);
			this.lbl_outlineColor_b.Name = "lbl_outlineColor_b";
			this.lbl_outlineColor_b.Size = new System.Drawing.Size(23, 12);
			this.lbl_outlineColor_b.TabIndex = 21;
			this.lbl_outlineColor_b.Text = "青：";
			// 
			// tb_outlineColor_b
			// 
			this.tb_outlineColor_b.Location = new System.Drawing.Point(45, 84);
			this.tb_outlineColor_b.Maximum = 255;
			this.tb_outlineColor_b.Name = "tb_outlineColor_b";
			this.tb_outlineColor_b.Size = new System.Drawing.Size(199, 45);
			this.tb_outlineColor_b.TabIndex = 20;
			this.tb_outlineColor_b.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_outlineColor_b.ValueChanged += new System.EventHandler(this.tb_outlineColor_b_ValueChanged);
			// 
			// tb_outlineColor_g
			// 
			this.tb_outlineColor_g.Location = new System.Drawing.Point(45, 51);
			this.tb_outlineColor_g.Maximum = 255;
			this.tb_outlineColor_g.Name = "tb_outlineColor_g";
			this.tb_outlineColor_g.Size = new System.Drawing.Size(199, 45);
			this.tb_outlineColor_g.TabIndex = 19;
			this.tb_outlineColor_g.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_outlineColor_g.ValueChanged += new System.EventHandler(this.tb_outlineColor_g_ValueChanged);
			// 
			// lbl_outlineColor_g
			// 
			this.lbl_outlineColor_g.AutoSize = true;
			this.lbl_outlineColor_g.Location = new System.Drawing.Point(16, 51);
			this.lbl_outlineColor_g.Name = "lbl_outlineColor_g";
			this.lbl_outlineColor_g.Size = new System.Drawing.Size(23, 12);
			this.lbl_outlineColor_g.TabIndex = 18;
			this.lbl_outlineColor_g.Text = "緑：";
			// 
			// lbl_outlineColor_r
			// 
			this.lbl_outlineColor_r.AutoSize = true;
			this.lbl_outlineColor_r.Location = new System.Drawing.Point(16, 18);
			this.lbl_outlineColor_r.Name = "lbl_outlineColor_r";
			this.lbl_outlineColor_r.Size = new System.Drawing.Size(23, 12);
			this.lbl_outlineColor_r.TabIndex = 17;
			this.lbl_outlineColor_r.Text = "赤：";
			// 
			// tb_outlineColor_r
			// 
			this.tb_outlineColor_r.Location = new System.Drawing.Point(45, 18);
			this.tb_outlineColor_r.Maximum = 255;
			this.tb_outlineColor_r.Name = "tb_outlineColor_r";
			this.tb_outlineColor_r.Size = new System.Drawing.Size(199, 45);
			this.tb_outlineColor_r.TabIndex = 0;
			this.tb_outlineColor_r.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_outlineColor_r.ValueChanged += new System.EventHandler(this.tb_outlineColor_r_ValueChanged);
			// 
			// grp_color
			// 
			this.grp_color.Controls.Add(this.lbl_color_b);
			this.grp_color.Controls.Add(this.tb_color_b);
			this.grp_color.Controls.Add(this.tb_color_g);
			this.grp_color.Controls.Add(this.lbl_color_g);
			this.grp_color.Controls.Add(this.lbl_color_r);
			this.grp_color.Controls.Add(this.tb_color_r);
			this.grp_color.Location = new System.Drawing.Point(9, 195);
			this.grp_color.Name = "grp_color";
			this.grp_color.Size = new System.Drawing.Size(250, 140);
			this.grp_color.TabIndex = 16;
			this.grp_color.TabStop = false;
			this.grp_color.Text = "色";
			// 
			// lbl_color_b
			// 
			this.lbl_color_b.AutoSize = true;
			this.lbl_color_b.Location = new System.Drawing.Point(16, 84);
			this.lbl_color_b.Name = "lbl_color_b";
			this.lbl_color_b.Size = new System.Drawing.Size(23, 12);
			this.lbl_color_b.TabIndex = 21;
			this.lbl_color_b.Text = "青：";
			// 
			// tb_color_b
			// 
			this.tb_color_b.Location = new System.Drawing.Point(45, 84);
			this.tb_color_b.Maximum = 255;
			this.tb_color_b.Name = "tb_color_b";
			this.tb_color_b.Size = new System.Drawing.Size(199, 45);
			this.tb_color_b.TabIndex = 20;
			this.tb_color_b.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_color_b.ValueChanged += new System.EventHandler(this.tb_color_b_ValueChanged);
			// 
			// tb_color_g
			// 
			this.tb_color_g.Location = new System.Drawing.Point(45, 51);
			this.tb_color_g.Maximum = 255;
			this.tb_color_g.Name = "tb_color_g";
			this.tb_color_g.Size = new System.Drawing.Size(199, 45);
			this.tb_color_g.TabIndex = 19;
			this.tb_color_g.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_color_g.ValueChanged += new System.EventHandler(this.tb_color_g_ValueChanged);
			// 
			// lbl_color_g
			// 
			this.lbl_color_g.AutoSize = true;
			this.lbl_color_g.Location = new System.Drawing.Point(16, 51);
			this.lbl_color_g.Name = "lbl_color_g";
			this.lbl_color_g.Size = new System.Drawing.Size(23, 12);
			this.lbl_color_g.TabIndex = 18;
			this.lbl_color_g.Text = "緑：";
			// 
			// lbl_color_r
			// 
			this.lbl_color_r.AutoSize = true;
			this.lbl_color_r.Location = new System.Drawing.Point(16, 18);
			this.lbl_color_r.Name = "lbl_color_r";
			this.lbl_color_r.Size = new System.Drawing.Size(23, 12);
			this.lbl_color_r.TabIndex = 17;
			this.lbl_color_r.Text = "赤：";
			// 
			// tb_color_r
			// 
			this.tb_color_r.Location = new System.Drawing.Point(45, 18);
			this.tb_color_r.Maximum = 255;
			this.tb_color_r.Name = "tb_color_r";
			this.tb_color_r.Size = new System.Drawing.Size(199, 45);
			this.tb_color_r.TabIndex = 0;
			this.tb_color_r.TickStyle = System.Windows.Forms.TickStyle.None;
			this.tb_color_r.ValueChanged += new System.EventHandler(this.tb_color_r_ValueChanged);
			// 
			// lbl_outline
			// 
			this.lbl_outline.AutoSize = true;
			this.lbl_outline.Location = new System.Drawing.Point(7, 169);
			this.lbl_outline.Name = "lbl_outline";
			this.lbl_outline.Size = new System.Drawing.Size(55, 12);
			this.lbl_outline.TabIndex = 15;
			this.lbl_outline.Text = "枠線太さ：";
			// 
			// txt_outline
			// 
			this.txt_outline.Location = new System.Drawing.Point(115, 166);
			this.txt_outline.Name = "txt_outline";
			this.txt_outline.Size = new System.Drawing.Size(200, 19);
			this.txt_outline.TabIndex = 14;
			this.txt_outline.TextChanged += new System.EventHandler(this.txt_outline_TextChanged);
			// 
			// txt_textureSize
			// 
			this.txt_textureSize.Location = new System.Drawing.Point(115, 141);
			this.txt_textureSize.Name = "txt_textureSize";
			this.txt_textureSize.Size = new System.Drawing.Size(200, 19);
			this.txt_textureSize.TabIndex = 13;
			this.txt_textureSize.TextChanged += new System.EventHandler(this.txt_textureSize_TextChanged);
			// 
			// lbl_textureSize
			// 
			this.lbl_textureSize.AutoSize = true;
			this.lbl_textureSize.Location = new System.Drawing.Point(7, 144);
			this.lbl_textureSize.Name = "lbl_textureSize";
			this.lbl_textureSize.Size = new System.Drawing.Size(83, 12);
			this.lbl_textureSize.TabIndex = 12;
			this.lbl_textureSize.Text = "テクスチャサイズ：";
			// 
			// txt_fontsize
			// 
			this.txt_fontsize.Location = new System.Drawing.Point(115, 116);
			this.txt_fontsize.Name = "txt_fontsize";
			this.txt_fontsize.Size = new System.Drawing.Size(200, 19);
			this.txt_fontsize.TabIndex = 11;
			this.txt_fontsize.TextChanged += new System.EventHandler(this.txt_fontsize_TextChanged);
			// 
			// lbl_fontsize
			// 
			this.lbl_fontsize.AutoSize = true;
			this.lbl_fontsize.Location = new System.Drawing.Point(7, 119);
			this.lbl_fontsize.Name = "lbl_fontsize";
			this.lbl_fontsize.Size = new System.Drawing.Size(73, 12);
			this.lbl_fontsize.TabIndex = 10;
			this.lbl_fontsize.Text = "フォントサイズ：";
			// 
			// txt_sheet
			// 
			this.txt_sheet.Location = new System.Drawing.Point(115, 91);
			this.txt_sheet.Name = "txt_sheet";
			this.txt_sheet.Size = new System.Drawing.Size(200, 19);
			this.txt_sheet.TabIndex = 9;
			this.txt_sheet.TextChanged += new System.EventHandler(this.txt_sheet_TextChanged);
			// 
			// lbl_sheet
			// 
			this.lbl_sheet.AutoSize = true;
			this.lbl_sheet.Location = new System.Drawing.Point(7, 94);
			this.lbl_sheet.Name = "lbl_sheet";
			this.lbl_sheet.Size = new System.Drawing.Size(51, 12);
			this.lbl_sheet.TabIndex = 8;
			this.lbl_sheet.Text = "シート名：";
			// 
			// btn_export
			// 
			this.btn_export.Location = new System.Drawing.Point(321, 64);
			this.btn_export.Name = "btn_export";
			this.btn_export.Size = new System.Drawing.Size(75, 23);
			this.btn_export.TabIndex = 7;
			this.btn_export.Text = "参照...";
			this.btn_export.UseVisualStyleBackColor = true;
			this.btn_export.Click += new System.EventHandler(this.btn_export_Click);
			// 
			// txt_export
			// 
			this.txt_export.Location = new System.Drawing.Point(115, 66);
			this.txt_export.Name = "txt_export";
			this.txt_export.ReadOnly = true;
			this.txt_export.Size = new System.Drawing.Size(200, 19);
			this.txt_export.TabIndex = 6;
			// 
			// lbl_export
			// 
			this.lbl_export.AutoSize = true;
			this.lbl_export.Location = new System.Drawing.Point(7, 69);
			this.lbl_export.Name = "lbl_export";
			this.lbl_export.Size = new System.Drawing.Size(96, 12);
			this.lbl_export.TabIndex = 5;
			this.lbl_export.Text = "出力先ディレクトリ：";
			// 
			// btn_txt
			// 
			this.btn_txt.Location = new System.Drawing.Point(321, 41);
			this.btn_txt.Name = "btn_txt";
			this.btn_txt.Size = new System.Drawing.Size(75, 23);
			this.btn_txt.TabIndex = 4;
			this.btn_txt.Text = "参照...";
			this.btn_txt.UseVisualStyleBackColor = true;
			this.btn_txt.Click += new System.EventHandler(this.btn_txt_Click);
			// 
			// txt_txt
			// 
			this.txt_txt.Location = new System.Drawing.Point(115, 43);
			this.txt_txt.Name = "txt_txt";
			this.txt_txt.ReadOnly = true;
			this.txt_txt.Size = new System.Drawing.Size(200, 19);
			this.txt_txt.TabIndex = 3;
			// 
			// lbl_txt
			// 
			this.lbl_txt.AutoSize = true;
			this.lbl_txt.Location = new System.Drawing.Point(7, 46);
			this.lbl_txt.Name = "lbl_txt";
			this.lbl_txt.Size = new System.Drawing.Size(81, 12);
			this.lbl_txt.TabIndex = 2;
			this.lbl_txt.Text = "テキストファイル：";
			// 
			// cb_font
			// 
			this.cb_font.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cb_font.FormattingEnabled = true;
			this.cb_font.Location = new System.Drawing.Point(115, 16);
			this.cb_font.Name = "cb_font";
			this.cb_font.Size = new System.Drawing.Size(200, 20);
			this.cb_font.TabIndex = 1;
			this.cb_font.SelectedIndexChanged += new System.EventHandler(this.cb_font_SelectedIndexChanged);
			// 
			// lbl_font
			// 
			this.lbl_font.AutoSize = true;
			this.lbl_font.Location = new System.Drawing.Point(7, 19);
			this.lbl_font.Name = "lbl_font";
			this.lbl_font.Size = new System.Drawing.Size(44, 12);
			this.lbl_font.TabIndex = 0;
			this.lbl_font.Text = "フォント：";
			// 
			// grp_preview
			// 
			this.grp_preview.Controls.Add(this.pic_preview);
			this.grp_preview.Location = new System.Drawing.Point(13, 364);
			this.grp_preview.Name = "grp_preview";
			this.grp_preview.Size = new System.Drawing.Size(259, 66);
			this.grp_preview.TabIndex = 1;
			this.grp_preview.TabStop = false;
			this.grp_preview.Text = "プレビュー";
			// 
			// pic_preview
			// 
			this.pic_preview.Location = new System.Drawing.Point(9, 19);
			this.pic_preview.Name = "pic_preview";
			this.pic_preview.Size = new System.Drawing.Size(244, 41);
			this.pic_preview.TabIndex = 0;
			this.pic_preview.TabStop = false;
			// 
			// btn_generate
			// 
			this.btn_generate.Location = new System.Drawing.Point(287, 373);
			this.btn_generate.Name = "btn_generate";
			this.btn_generate.Size = new System.Drawing.Size(75, 23);
			this.btn_generate.TabIndex = 2;
			this.btn_generate.Text = "生成";
			this.btn_generate.UseVisualStyleBackColor = true;
			this.btn_generate.Click += new System.EventHandler(this.btn_generate_Click);
			// 
			// btn_saveSetting
			// 
			this.btn_saveSetting.Location = new System.Drawing.Point(368, 373);
			this.btn_saveSetting.Name = "btn_saveSetting";
			this.btn_saveSetting.Size = new System.Drawing.Size(75, 23);
			this.btn_saveSetting.TabIndex = 3;
			this.btn_saveSetting.Text = "設定セーブ";
			this.btn_saveSetting.UseVisualStyleBackColor = true;
			this.btn_saveSetting.Click += new System.EventHandler(this.btn_saveSetting_Click);
			// 
			// btn_loadSetting
			// 
			this.btn_loadSetting.Location = new System.Drawing.Point(449, 373);
			this.btn_loadSetting.Name = "btn_loadSetting";
			this.btn_loadSetting.Size = new System.Drawing.Size(75, 23);
			this.btn_loadSetting.TabIndex = 4;
			this.btn_loadSetting.Text = "設定ロード";
			this.btn_loadSetting.UseVisualStyleBackColor = true;
			this.btn_loadSetting.Click += new System.EventHandler(this.btn_loadSetting_Click);
			// 
			// timer
			// 
			this.timer.Tick += new System.EventHandler(this.timer_Tick);
			// 
			// Window
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.Controls.Add(this.btn_loadSetting);
			this.Controls.Add(this.btn_saveSetting);
			this.Controls.Add(this.btn_generate);
			this.Controls.Add(this.grp_preview);
			this.Controls.Add(this.grp_font);
			this.Name = "Window";
			this.Text = "フォントジェネレーター";
			this.Load += new System.EventHandler(this.Window_Load);
			this.grp_font.ResumeLayout(false);
			this.grp_font.PerformLayout();
			this.grp_outlineColor.ResumeLayout(false);
			this.grp_outlineColor.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_b)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_g)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_outlineColor_r)).EndInit();
			this.grp_color.ResumeLayout(false);
			this.grp_color.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_b)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_g)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.tb_color_r)).EndInit();
			this.grp_preview.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pic_preview)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox grp_font;
		private System.Windows.Forms.Label lbl_font;
		private System.Windows.Forms.ComboBox cb_font;
		private System.Windows.Forms.Button btn_txt;
		private System.Windows.Forms.TextBox txt_txt;
		private System.Windows.Forms.Label lbl_txt;
		private System.Windows.Forms.Button btn_export;
		private System.Windows.Forms.TextBox txt_export;
		private System.Windows.Forms.Label lbl_export;
		private System.Windows.Forms.TextBox txt_sheet;
		private System.Windows.Forms.Label lbl_sheet;
		private System.Windows.Forms.TextBox txt_fontsize;
		private System.Windows.Forms.Label lbl_fontsize;
		private System.Windows.Forms.TextBox txt_textureSize;
		private System.Windows.Forms.Label lbl_textureSize;
		private System.Windows.Forms.Label lbl_outline;
		private System.Windows.Forms.TextBox txt_outline;
		private System.Windows.Forms.GroupBox grp_color;
		private System.Windows.Forms.TrackBar tb_color_r;
		private System.Windows.Forms.Label lbl_color_b;
		private System.Windows.Forms.TrackBar tb_color_b;
		private System.Windows.Forms.TrackBar tb_color_g;
		private System.Windows.Forms.Label lbl_color_g;
		private System.Windows.Forms.Label lbl_color_r;
		private System.Windows.Forms.GroupBox grp_outlineColor;
		private System.Windows.Forms.Label lbl_outlineColor_b;
		private System.Windows.Forms.TrackBar tb_outlineColor_b;
		private System.Windows.Forms.TrackBar tb_outlineColor_g;
		private System.Windows.Forms.Label lbl_outlineColor_g;
		private System.Windows.Forms.Label lbl_outlineColor_r;
		private System.Windows.Forms.TrackBar tb_outlineColor_r;
		private System.Windows.Forms.GroupBox grp_preview;
		private System.Windows.Forms.Button btn_generate;
		private System.Windows.Forms.Button btn_saveSetting;
		private System.Windows.Forms.Button btn_loadSetting;
		private System.Windows.Forms.PictureBox pic_preview;
		private System.Windows.Forms.Timer timer;
	}
}