using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs.Window
{
	/// <summary>
	/// ウインドウを表示する。
	/// </summary>
	class Empty : ISample
	{
		public void Run()
		{
			// 初期設定を行う。
			var option = new ace.EngineOption
			{
				GraphicsType = ace.GraphicsType.DirectX11,
				IsFullScreen = false
			};

			// aceを初期化する。
			ace.Engine.Initialize("Empty", 640, 480, option);

			// aceが進行可能かチェックする。
			while (ace.Engine.DoEvents())
			{
				// aceを更新する。
				ace.Engine.Update();
			}

			// aceを終了する。
			ace.Engine.Terminate();
		}
	}
}
