
// Disposeが呼ばれた時や破棄されたときにコンソールに出力するオブジェクト。
class Dispose_Object2D_MessageObject : asd.TextureObject2D
{
	// Disposeが呼ばれたときに呼ばれる。
	protected override void OnDispose()
	{
		System.Console.WriteLine("MessageObject.OnDispose");
	}
}

class Dispose_Object2D : ISample
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
		get { return "Dispose_Object2D"; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Dispose_Object2D", 640, 480, new asd.EngineOption());

		// 画像を読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Picture1.png");

		// カスタマイズしたオブジェクトのインスタンスを生成する。
		var obj = new Dispose_Object2D_MessageObject();

		// オブジェクトの位置とテクスチャを設定する。
		obj.Position = new asd.Vector2DF(50, 50);
		obj.Texture = texture;

		// 描画するオブジェクトをエンジンに追加する。
		asd.Engine.AddObject2D(obj);

		// フレーム数を数えるための変数。
		int count = 0;

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();

			// フレームを数える。
			count++;

			// 10フレーム目になったら
			if (count == 10)
			{
				System.Console.WriteLine("Objectを破棄します");
				// オブジェクトを破棄する。
				obj.Dispose();
			}
			Recorder.TakeScreenShot("Dispose_Object2D", 30);
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}