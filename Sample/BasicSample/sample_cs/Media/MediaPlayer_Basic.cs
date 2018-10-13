
/// <summary>
/// 映像を表示するサンプル
/// </summary>
class MediaPlayer_Basic : ISample
{
    public string Description
    {
        get { return ""; }
    }
	public string Title
	{
		get { return ""; }
	}
	public string ClassName
	{
		get { return "MediaPlayer_Basic"; }
	}

    public unsafe void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("MediaPlayer_Basic", 640, 480, new asd.EngineOption());

		// 空の画像を生成する。
		asd.Texture2D texture = asd.Engine.Graphics.CreateEmptyTexture2D(640, 480, asd.TextureFormat.R8G8B8A8_UNORM_SRGB);

		// 画像描画オブジェクトのインスタンスを生成する。
		asd.TextureObject2D obj = new asd.TextureObject2D();

		// 描画される画像を設定する。
		obj.Texture = texture;

		// 画像描画オブジェクトのインスタンスをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		// メディアプレイヤーを生成する。
		asd.MediaPlayer mp = asd.Engine.Graphics.CreateMediaPlayer();

		// 映像を読み込む。
		mp.Load("Data/Movie/Sample1.mp4");

		// 映像を再生する。
		mp.Play();

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// 映像を画像にコピーする。
			mp.WriteToTexture2D(texture);

			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("MediaPlayer_Basic", 50);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}