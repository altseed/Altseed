using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Window
{
	/// <summary>
	/// 外部のウインドウにaceの機能で描画を行う。
	/// </summary>
	class EmptyExternal : ISample
	{
		public void Run()
		{
			bool closed = false;
			System.Windows.Forms.Form form = new System.Windows.Forms.Form();
			form.FormClosed += (object sender, System.Windows.Forms.FormClosedEventArgs e) =>
				{
					closed = true;
				};
			form.Show();


			// aceを初期化する。
			ace.Engine.InitializeByExternalWindow(form.Handle, new IntPtr(), form.Size.Width, form.Size.Height);

			// aceが進行可能かチェックする。
			while (ace.Engine.DoEvents())
			{
				System.Windows.Forms.Application.DoEvents();
				if (closed) break;

				// aceを更新する。
				ace.Engine.Update();
			}

			// aceを終了する。
			ace.Engine.Terminate();
		}
	}
}
