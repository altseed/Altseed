
/// <summary>
/// 空の画像を編集するサンプル
/// </summary>
class Texture_Basic : ISample
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
		get { return "Texture_Basic"; }
	}

    public unsafe void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Texture_Basic", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		asd.Texture2D texture = asd.Engine.Graphics.CreateEmptyTexture2D(256, 256, asd.TextureFormat.R8G8B8A8_UNORM_SRGB);

		// ロックして編集可能な状態にする。
		asd.TextureLockInfomation lockInfo = new asd.TextureLockInfomation();
		if (texture.Lock(lockInfo))
		{
#if LANG_CS
			// C#のみの高速処理
			for (int y = 0; y < lockInfo.Size.Y; y++)
			{
				for (int x = 0; x < lockInfo.Size.X; x++)
				{
					var pixel = &((byte*)lockInfo.Pixels)[(x + y * lockInfo.Size.X) * lockInfo.Pitch];
					pixel[0] = (byte)x;
					pixel[1] = (byte)y;
					pixel[2] = 0;
					pixel[3] = 255;
				}
			}
#else
			System.Console.WriteLine("実装されていません。");
#endif
			// Unlockして編集結果を適用する。
			texture.Unlock();
		}

		// 画像描画オブジェクトのインスタンスを生成する。
		asd.TextureObject2D obj = new asd.TextureObject2D();

		// 描画される画像を設定する。
		obj.Texture = texture;

		// 描画位置を指定する。
		obj.Position = new asd.Vector2DF(50, 50);

		// 画像描画オブジェクトのインスタンスをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("Texture_Basic", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}