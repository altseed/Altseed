using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/// <summary>
/// 図形を表示する。
/// </summary>
class Geometry2D_RectangleShape : ISample
{
	public void Run()
	{
		// aceを初期化する
		asd.Engine.Initialize("Geometry2D_RectangleShape", 640, 480, new asd.EngineOption());

		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png");

		//図形描画クラスのコンストラクタを呼び出す
		var geometryObj = new asd.GeometryObject2D();

		//図形描画クラスをレイヤーに追加する。
		asd.Engine.AddObject2D(geometryObj);

		//矩形を図形描画クラス2にて描画する。
		{
			//矩形の描画範囲、UV範囲を指定。
			var rect = new asd.RectangleShape();
			rect.DrawingArea = new asd.RectF(10, 210, 300, 200);
			rect.UV = new asd.RectF(0, 0, 0.5f, 0.5f);

			//矩形を描画する図形として設定し、合成するテクスチャも設定。
			geometryObj.Shape = rect;
			geometryObj.Texture = texture;
			geometryObj.Position = new asd.Vector2DF(0, 0);
		}

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
