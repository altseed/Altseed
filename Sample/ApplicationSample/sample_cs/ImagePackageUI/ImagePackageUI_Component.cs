
/// <summary>
/// ImagePackageを用いてUIを配置して、特定の名前の画像をアニメーションさせるサンプル。
/// </summary>
class ImagePackageUI_Component : ISample
{
	class AlphaAnimationComponent : asd.Object2DComponent
	{
		int time = 0;
		protected override void OnUpdate()
		{
			// アルファ値をsinカーブでアニメーションするようにする。
			var alpha = (byte)((float)(System.Math.Sin(time / 20.0f) + 1.0f) / 2.0f * 255);
			var owner = (asd.TextureObject2D)Owner;
			owner.Color = new asd.Color(255, 255, 255, alpha);
			time++;
		}
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("ImagePackageUI_Component", 640, 480, new asd.EngineOption());

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
			asd.Engine.AddObject2D(textureObject2D);

			// Background_Lightという名称の画像にコンポーネントを適用する。
			if (imagePackage.GetImageName(i) == "Background_Light")
			{
				textureObject2D.AlphaBlend = asd.AlphaBlendMode.Add;
				textureObject2D.AddComponent(new AlphaAnimationComponent(), "AlphaAnimation");
			}
		}

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.CaptureScreen("ImagePackageUI_Component", 5, 16, 0.25f, 0.5f);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}

