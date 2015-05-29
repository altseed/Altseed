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
			var option = new ace.EngineOption
			{
				IsFullScreen = false
			};

			// AC-Engineを初期化する。
			ace.Engine.Initialize("Empty", 640, 480, option);

			// AC-Engineのウインドウが閉じられていないか確認する。
			while (ace.Engine.DoEvents())
			{
				// AC-Engineを更新する。
				ace.Engine.Update();
			}

			// AC-Engineの終了処理をする。
			ace.Engine.Terminate();
		}
	}
}
