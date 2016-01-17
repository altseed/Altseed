
/// <summary>
/// 読み込まれた画像を編集するサンプル
/// </summary>
class Texture_Edit : ISample
{
    public string Description
    {
        get { return ""; }
    }

    public unsafe void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Texture_Edit", 640, 480, new asd.EngineOption());

		// 画像を編集可能な状態で読み込む。
		asd.Texture2D texture = asd.Engine.Graphics.CreateEditableTexture2D("Data/Texture/Picture1.png");

		// ロックして編集可能な状態にする。
		asd.TextureLockInfomation lockInfo = new asd.TextureLockInfomation();
		if (texture.Lock(lockInfo))
		{
			for (int y = 0; y < lockInfo.Size.Y; y++)
			{
				for (int x = 0; x < lockInfo.Size.X; x++)
				{
					var pixel = &((byte*)lockInfo.Pixels)[(x + y * lockInfo.Size.X) * lockInfo.Pitch];
					pixel[1] = (byte)(y / 2);
				}
			}

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
			Recorder.TakeScreenShot("Texture_Edit", 5);
		}

		// Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}