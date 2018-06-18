namespace ImagePackageGenerator
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
			this.lst_layers = new System.Windows.Forms.ListBox();
			this.pic = new System.Windows.Forms.PictureBox();
			this.statusStrip = new System.Windows.Forms.StatusStrip();
			this.toolStripStatus = new System.Windows.Forms.ToolStripStatusLabel();
			this.menuStrip = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			((System.ComponentModel.ISupportInitialize)(this.pic)).BeginInit();
			this.statusStrip.SuspendLayout();
			this.menuStrip.SuspendLayout();
			this.SuspendLayout();
			// 
			// lst_layers
			// 
			this.lst_layers.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
			this.lst_layers.Font = new System.Drawing.Font("メイリオ", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.lst_layers.FormattingEnabled = true;
			this.lst_layers.ItemHeight = 18;
			this.lst_layers.Location = new System.Drawing.Point(12, 30);
			this.lst_layers.Name = "lst_layers";
			this.lst_layers.Size = new System.Drawing.Size(120, 382);
			this.lst_layers.TabIndex = 0;
			this.lst_layers.SelectedValueChanged += new System.EventHandler(this.lst_layers_SelectedValueChanged);
			// 
			// pic
			// 
			this.pic.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pic.Location = new System.Drawing.Point(138, 30);
			this.pic.Name = "pic";
			this.pic.Size = new System.Drawing.Size(474, 382);
			this.pic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.pic.TabIndex = 1;
			this.pic.TabStop = false;
			// 
			// statusStrip
			// 
			this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatus});
			this.statusStrip.Location = new System.Drawing.Point(0, 420);
			this.statusStrip.Name = "statusStrip";
			this.statusStrip.Size = new System.Drawing.Size(624, 22);
			this.statusStrip.TabIndex = 2;
			this.statusStrip.Text = "statusStrip1";
			// 
			// toolStripStatus
			// 
			this.toolStripStatus.Name = "toolStripStatus";
			this.toolStripStatus.Size = new System.Drawing.Size(0, 17);
			// 
			// menuStrip
			// 
			this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
			this.menuStrip.Location = new System.Drawing.Point(0, 0);
			this.menuStrip.Name = "menuStrip";
			this.menuStrip.Size = new System.Drawing.Size(624, 26);
			this.menuStrip.TabIndex = 3;
			this.menuStrip.Text = "menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.exportToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(68, 22);
			this.fileToolStripMenuItem.Text = "ファイル";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.openToolStripMenuItem.Text = "開く";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
			// 
			// exportToolStripMenuItem
			// 
			this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
			this.exportToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.exportToolStripMenuItem.Text = "出力";
			this.exportToolStripMenuItem.Click += new System.EventHandler(this.exportToolStripMenuItem_Click);
			// 
			// Window
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(624, 442);
			this.Controls.Add(this.statusStrip);
			this.Controls.Add(this.menuStrip);
			this.Controls.Add(this.pic);
			this.Controls.Add(this.lst_layers);
			this.MainMenuStrip = this.menuStrip;
			this.Name = "Window";
			this.Text = "イメージパッケージジェネレーター";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Window_FormClosed);
			this.Load += new System.EventHandler(this.Window_Load);
			((System.ComponentModel.ISupportInitialize)(this.pic)).EndInit();
			this.statusStrip.ResumeLayout(false);
			this.statusStrip.PerformLayout();
			this.menuStrip.ResumeLayout(false);
			this.menuStrip.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListBox lst_layers;
		private System.Windows.Forms.PictureBox pic;
		private System.Windows.Forms.StatusStrip statusStrip;
		private System.Windows.Forms.ToolStripStatusLabel toolStripStatus;
		private System.Windows.Forms.MenuStrip menuStrip;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
	}
}