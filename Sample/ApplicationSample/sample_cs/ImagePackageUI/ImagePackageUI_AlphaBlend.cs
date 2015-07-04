
/// <summary>
/// ImagePackageを用いてUIを配置して、特定の名前の画像のアルファブレンドの方法を変更するサンプル。
/// </summary>
class ImagePackageUI_AlphaBlend : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("ImagePackageUI_AlphaBlend", 640, 480, new asd.EngineOption());

		// シーンを生成する
		var scene = new asd.Scene();

		// レイヤーを生成する
		var layer = new asd.Layer2D();

		// シーンにレイヤーを追加する
		scene.AddLayer(layer);

		// シーンを切り替える
		asd.Engine.ChangeScene(scene);

		// イメージパッケージを読み込む
		asd.ImagePackage imagePackage = asd.Engine.Graphics.CreateImagePackage("Data/ImagePackage/UI.aip");

		for (int i = 0; i < imagePackage.ImageCount; i++)
		{
			// テクスチャを取り出す
			asd.Texture2D texture = imagePackage.GetImage(i);
			asd.RectI area = imagePackage.GetImageArea(i);

			// テクスチャをオブジェクトとして追加する
			asd.TextureObject2D textureObject2D = new asd.TextureObject2D();
			textureObject2D.Texture = texture;
			textureObject2D.Position = new asd.Vector2DF(area.X, area.Y);
			layer.AddObject(textureObject2D);

			// Background_Lightという名称の画像のアルファブレンドの方法を変更する。
			if(imagePackage.GetImageName(i) == "Background_Light")
			{
				textureObject2D.AlphaBlend = asd.AlphaBlendMode.Add;
			}
		}

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("ImagePackageUI_AlphaBlend", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

