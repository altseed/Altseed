
namespace sample_cs
{
	class File_PackFile : ISample
    {
        public string Description
        {
            get { return ""; }
        }
		public string Title
		{
			get { return ""; }
		}

        // PackFileを用いて、パッケージから画像を表示するサンプルです。
        public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("File_PackFile", 640, 480, new asd.EngineOption());

			// パッケージをルートディレクトリに追加する。
			asd.Engine.File.AddRootDirectory("Data.pack");

			// オブジェクトを生成する。
			var obj = new asd.TextureObject2D();

			// パッケージ内の画像を読み込む。
			var texture = asd.Engine.Graphics.CreateTexture2D("Texture/Picture1.png");

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
				Recorder.TakeScreenShot("File_PackFile", 30);
			}

			//Altseedの終了処理をする。
			asd.Engine.Terminate();
		}
	}
}
