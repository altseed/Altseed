
class File_RootDirectory : ISample
{
	public string Description
	{
		get { return "ファイルを探す際のルートディレクトリを設定する機能のサンプル。"; }
	}
	public string Title
	{
		get { return "ルートディレクトリの設定"; }
	}
	public string ClassName
	{
		get { return "File_RootDirectory"; }
	}

	// AddRootDirectory を用いて、ファイルの起点となる場所を指定。
	// その起点を基準とした相対パスで画像を指定し描画するサンプル。
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("File_RootDirectory", 640, 480, new asd.EngineOption());

		// ルートディレクトリを追加する。
		asd.Engine.File.AddRootDirectory("Data/Texture");

		// オブジェクトを生成する。
		var obj = new asd.TextureObject2D();

		// 画像を読み込む。
		var texture = asd.Engine.Graphics.CreateTexture2D("Picture1.png");

		// オブジェクトに画像を設定する。
		obj.Texture = texture;

		// オブジェクトの位置を設定する。
		obj.Position = new asd.Vector2DF(100, 100);

		// エンジンにオブジェクトを追加する。
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("File_RootDirectory", 30);
		}

		//Altseedの終了処理をする。
		asd.Engine.Terminate();
	}
}