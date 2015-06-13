using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Window
{
	/// <summary>
	/// ウインドウを表示する。
	/// </summary>
	class Empty : ISample
	{
		public void Run()
		{
			// 初期設定を行う。
			var option = new asd.EngineOption
			{
				IsFullScreen = false
			};

			// aceを初期化する。
			asd.Engine.Initialize("Empty", 640, 480, option);

			// aceが進行可能かチェックする。
			while (asd.Engine.DoEvents())
			{
				// aceを更新する。
				asd.Engine.Update();
			}

			// aceを終了する。
			asd.Engine.Terminate();
		}
	}
}
