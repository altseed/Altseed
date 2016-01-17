using System.Text;

namespace sample_cs
{
	class File_StaticFile : ISample
    {
        public string Description
        {
            get { return ""; }
        }

        // StaticFile を用いて、ファイルからテキストを読み込むサンプルです。
        public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("File_StaticFile", 640, 480, new asd.EngineOption());

			// フォントを生成する。
			var font = asd.Engine.Graphics.CreateFont("Data/Font/Font1.aff");

			// オブジェクトを生成する。
			var obj = new asd.TextObject2D();

			// 描画に使うフォントを設定する
			obj.Font = font;

			// 描画位置を指定する
			obj.Position = new asd.Vector2DF(100, 100);

			// ファイルオブジェクト作成
			var staticFile = asd.Engine.File.CreateStaticFile("Data/Text/HelloWorld.txt");

			// 描画したいテキストのバイト列をファイルから読み取る
			byte[] textBytes = staticFile.Buffer;

			// UTF-8でバイト列をデコードする
			char[] text = Encoding.UTF8.GetChars(textBytes);

			// 描画する文字列の指定
			obj.Text = string.Concat(text);

			// オブジェクトのインスタンスをエンジンへ追加する。
			asd.Engine.AddObject2D(obj);

			// Altseedのウインドウが閉じられていないか確認する。
			while (asd.Engine.DoEvents())
			{
				// Altseedを更新する。
				asd.Engine.Update();
				Recorder.TakeScreenShot("File_StaticFile", 30);
			}

			// Altseedを終了する。
			asd.Engine.Terminate();
		}
	}
}
