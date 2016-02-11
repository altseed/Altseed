
/// <summary>
/// 画像を変形させて表示するサンプル。
/// </summary>
class TextureObject2D_Transform : ISample
{
    public string Description
    {
        get { return ""; }
    }
	public string Title
	{
		get { return ""; }
	}

    public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("TextureObject2D_Transform", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		asd.Texture2D texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// 画像描画オブジェクトのインスタンスを生成する。
		asd.TextureObject2D obj = new asd.TextureObject2D();

		// 描画される画像を設定する。
		obj.Texture = texture;

		// 描画位置を指定する。
		obj.Position = new asd.Vector2DF(320, 240);

		// 画像(サイズ 512 x 512)の中心(256 x 256 の地点)を描画の起点とする。
		obj.CenterPosition = new asd.Vector2DF(256, 256);

		// 中心を軸に画像を45度回転する。
		obj.Angle = 45;

		// 画像をX,Y方向に0.4倍に縮小する。
		obj.Scale = new asd.Vector2DF(0.4f, 0.4f);

		// 画像描画オブジェクトのインスタンスをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("TextureObject2D_Transform", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}