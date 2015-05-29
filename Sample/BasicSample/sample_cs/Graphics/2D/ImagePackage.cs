
/// <summary>
/// ImagePackageのサンプル。画像パッケージを元に画像を配置しています。
/// </summary>
class ImagePackage : ISample
{
	public void Run()
	{
		// AC-Engineを初期化する。
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

