using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Graphics._2D
{
	/// <summary>
	/// 画像パッケージから画像を配置する
	/// </summary>
	class ImagePackage : ISample
	{
		public void Run()
		{
			// aceを初期化する
			asd.Engine.Initialize("ImagePackage", 640, 480, new asd.EngineOption());

			// シーンを生成する
			var scene = new asd.Scene();

			// レイヤーを生成する
			var layer = new asd.Layer2D();

			// シーンにレイヤーを追加する
			scene.AddLayer(layer);

			// シーンを切り替える
			asd.Engine.ChangeScene(scene);

			// イメージパッケージを読み込む
			asd.ImagePackage imagePackage = asd.Engine.Graphics.CreateImagePackage("Data/ImagePackage/Game.aip");

			for(int i = 0; i < imagePackage.ImageCount; i++)
			{
				// テクスチャを取り出す
				asd.Texture2D texture = imagePackage.GetImage(i);
				asd.RectI area = imagePackage.GetImageArea(i);

				// テクスチャをオブジェクトとして追加する
				asd.TextureObject2D textureObject2D = new asd.TextureObject2D();
				textureObject2D.Texture = texture;
				textureObject2D.Position = new asd.Vector2DF(area.X, area.Y);
				layer.AddObject(textureObject2D);
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
}
