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
			ace.Engine.Initialize("ImagePackage", 640, 480, new ace.EngineOption());

			// シーンを生成する
			var scene = new ace.Scene();

			// レイヤーを生成する
			var layer = new ace.Layer2D();

			// シーンにレイヤーを追加する
			scene.AddLayer(layer);

			// シーンを切り替える
			ace.Engine.ChangeScene(scene);

			// イメージパッケージを読み込む
			ace.ImagePackage imagePackage = ace.Engine.Graphics.CreateImagePackage("Data/ImagePackage/Game.aip");

			for(int i = 0; i < imagePackage.ImageCount; i++)
			{
				// テクスチャを取り出す
				ace.Texture2D texture = imagePackage.GetImage(i);
				ace.RectI area = imagePackage.GetImageArea(i);

				// テクスチャをオブジェクトとして追加する
				ace.TextureObject2D textureObject2D = new ace.TextureObject2D();
				textureObject2D.Texture = texture;
				textureObject2D.Position = new ace.Vector2DF(area.X, area.Y);
				layer.AddObject(textureObject2D);
			}

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
