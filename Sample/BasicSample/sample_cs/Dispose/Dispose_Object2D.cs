using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
	class Dispose_Object2D : ISample
	{
		// Vanishが呼ばれた時や破棄されたときにコンソールに出力するオブジェクト。
		class MessageObject : asd.TextureObject2D
		{
			// Vanishが呼ばれたときに呼ばれる。
			protected override void OnVanish()
			{
				Console.WriteLine("MessageObject.OnVanish");
			}

			// このオブジェクトが破棄されたときに呼ばれる。
			protected override void OnDispose()
			{
				Console.WriteLine("MessageObject.OnDispose");
			}
		}

		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Dispose_Object2D", 640, 480, new asd.EngineOption());

			// 画像を読み込む。
			var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

			// カスタマイズしたオブジェクトのインスタンスを生成する。
			var obj = new MessageObject();

			// オブジェクトの位置とテクスチャを設定する。
			obj.Position = new asd.Vector2DF(50, 50);
			obj.Texture = texture;

			// 描画するオブジェクトをエンジンに追加する。
			asd.Engine.AddObject2D(obj);

			// フレーム数を数えるための変数。
			int count = 0;

			// Altseedのウインドウが閉じられていないか確認する。
			while(asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();

				// フレームを数える。
				++count;

				// 10フレーム目になったら
				if(count == 10)
				{
					Console.WriteLine("Objectを破棄します");
					// オブジェクトを破棄する。
					obj.Vanish();
				}
			}

			// Altseedを終了する。
			asd.Engine.Terminate();
		}
	}
}
