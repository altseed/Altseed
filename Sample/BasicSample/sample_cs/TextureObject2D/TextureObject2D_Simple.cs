
/// <summary>
/// 画像を表示するサンプル。
/// </summary>
class TextureObject2D_Simple : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("TextureObject2D_Simple", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		asd.Texture2D texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// TextureObject2Dのインスタンスを生成する。
		asd.TextureObject2D obj = new asd.TextureObject2D();

		// 描画される画像を設定する。
		obj.Texture = texture;

		// 描画位置を指定する。
		obj.Position = new asd.Vector2DF(50, 50);

		// オブジェクトのインスタンスをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("TextureObject2D_Simple", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}