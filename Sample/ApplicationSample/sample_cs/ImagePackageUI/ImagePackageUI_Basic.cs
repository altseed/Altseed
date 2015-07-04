
/// <summary>
/// ImagePackageを用いてUIを配置するサンプル。
/// </summary>
class ImagePackageUI_Basic : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("ImagePackageUI_Basic", 640, 480, new asd.EngineOption());

		// イメージパッケージを読み込む
		asd.ImagePackage imagePackage = asd.Engine.Graphics.CreateImagePackage("Data/ImagePackage/UI.aip");

		for(int i = 0; i < imagePackage.ImageCount; i++)
		{
			// テクスチャを取り出す
			asd.Texture2D texture = imagePackage.GetImage(i);
			asd.RectI area = imagePackage.GetImageArea(i);

			// テクスチャをオブジェクトとして追加する
			asd.TextureObject2D textureObject2D = new asd.TextureObject2D();
			textureObject2D.Texture = texture;
			textureObject2D.Position = new asd.Vector2DF(area.X, area.Y);
			asd.Engine.AddObject2D(textureObject2D);
		}

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("ImagePackageUI_Basic", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

