namespace FilePackageGenerator.GUI
{
	partial class MainWindow
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
			this.lbl_ref = new System.Windows.Forms.Label();
			this.btn_ref = new System.Windows.Forms.Button();
			this.lbl_password = new System.Windows.Forms.Label();
			this.txt_password = new System.Windows.Forms.TextBox();
			this.btn_export = new System.Windows.Forms.Button();
			this.lbl_target = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// lbl_ref
			// 
			this.lbl_ref.AutoSize = true;
			this.lbl_ref.Location = new System.Drawing.Point(12, 9);
			this.lbl_ref.Name = "lbl_ref";
			this.lbl_ref.Size = new System.Drawing.Size(63, 12);
			this.lbl_ref.TabIndex = 0;
			this.lbl_ref.Text = "読み込み元";
			// 
			// btn_ref
			// 
			this.btn_ref.Location = new System.Drawing.Point(81, 4);
			this.btn_ref.Name = "btn_ref";
			this.btn_ref.Size = new System.Drawing.Size(75, 23);
			this.btn_ref.TabIndex = 1;
			this.btn_ref.Text = "参照";
			this.btn_ref.UseVisualStyleBackColor = true;
			this.btn_ref.Click += new System.EventHandler(this.btn_ref_Click);
			// 
			// lbl_password
			// 
			this.lbl_password.AutoSize = true;
			this.lbl_password.Location = new System.Drawing.Point(12, 38);
			this.lbl_password.Name = "lbl_password";
			this.lbl_password.Size = new System.Drawing.Size(52, 12);
			this.lbl_password.TabIndex = 2;
			this.lbl_password.Text = "パスワード";
			// 
			// txt_password
			// 
			this.txt_password.Location = new System.Drawing.Point(81, 35);
			this.txt_password.Name = "txt_password";
			this.txt_password.Size = new System.Drawing.Size(200, 19);
			this.txt_password.TabIndex = 3;
			// 
			// btn_export
			// 
			this.btn_export.Location = new System.Drawing.Point(14, 75);
			this.btn_export.Name = "btn_export";
			this.btn_export.Size = new System.Drawing.Size(75, 23);
			this.btn_export.TabIndex = 4;
			this.btn_export.Text = "出力";
			this.btn_export.UseVisualStyleBackColor = true;
			this.btn_export.Click += new System.EventHandler(this.btn_export_Click);
			// 
			// lbl_target
			// 
			this.lbl_target.AutoSize = true;
			this.lbl_target.Location = new System.Drawing.Point(162, 9);
			this.lbl_target.Name = "lbl_target";
			this.lbl_target.Size = new System.Drawing.Size(0, 12);
			this.lbl_target.TabIndex = 5;
			// 
			// MainWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(464, 282);
			this.Controls.Add(this.lbl_target);
			this.Controls.Add(this.btn_export);
			this.Controls.Add(this.txt_password);
			this.Controls.Add(this.lbl_password);
			this.Controls.Add(this.btn_ref);
			this.Controls.Add(this.lbl_ref);
			this.Name = "MainWindow";
			this.Text = "ファイルパッケージジェネレーター";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label lbl_ref;
		private System.Windows.Forms.Button btn_ref;
		private System.Windows.Forms.Label lbl_password;
		private System.Windows.Forms.TextBox txt_password;
		private System.Windows.Forms.Button btn_export;
		private System.Windows.Forms.Label lbl_target;
	}
}