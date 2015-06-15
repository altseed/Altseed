using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 弧を表示するサンプル。
/// </summary>
class GeometryObject2D_ArcShape : ISample
{
	public void Run()
	{
		// Altseedを初期化する
		asd.Engine.Initialize("Geometry2D_ArcShape", 640, 480, new asd.EngineOption());

		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

		//図形描画クラスのコンストラクタを呼び出す
		var geometryObj = new asd.GeometryObject2D();

		//図形描画クラスをレイヤーに追加する。
		asd.Engine.AddObject2D(geometryObj);

		// Altseedを更新する。
		while (asd.Engine.DoEvents())
		{
			// aceを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("Geometry2D_ArcShape", 5);
		}

		// aceを終了する。
		asd.Engine.Terminate();
	}
}
