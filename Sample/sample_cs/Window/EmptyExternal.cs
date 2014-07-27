using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Window
{
	/// <summary>
	/// 外部のウインドウにaceの機能で描画を行う。
	/// </summary>
	class EmptyExternal : ISample
	{
		public void Run()
		{
			// 初期設定を行う。
			var option = new ace.EngineOption
			{
				IsFullScreen = false
			};

			bool closed = false;
			System.Windows.Forms.Form form = new System.Windows.Forms.Form();
			form.FormClosed += (object sender, System.Windows.Forms.FormClosedEventArgs e) =>
				{
					closed = true;
				};
			form.Show();


			// aceを初期化する。
			ace.Engine.InitializeByExternalWindow(form.Handle, IntPtr.Zero, form.Size.Width, form.Size.Height, option);

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
