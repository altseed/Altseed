using System.Collections.Generic;
using System.Text;

class File_StreamFile : ISample
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
		get { return "File_StreamFile"; }
	}

	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("File_StreamFile", 640, 480, new asd.EngineOption());

		// フォントを生成する。
		var font = asd.Engine.Graphics.CreateFont("Data/Font/Font1.aff");

		// オブジェクトを生成する。
		var obj = new asd.TextObject2D();

		// 描画に使うフォントを設定する
		obj.Font = font;

		// 描画位置を指定する
		obj.Position = new asd.Vector2DF(100, 100);

		// ファイルオブジェクト作成
		var staticFile = asd.Engine.File.CreateStreamFile("Data/Text/HelloWorld.txt");

		// ファイルの内容をバッファへ格納
		var buffer = new List<byte>();
		staticFile.Read(buffer, 32);

		// UTF-8でバイト列をデコードする
		char[] text = Encoding.UTF8.GetChars(buffer.ToArray());

		// 描画する文字列の指定
		obj.Text = string.Concat(text);

		// オブジェクトのインスタンスをエンジンへ追加する。
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while (asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
			Recorder.TakeScreenShot("File_StreamFile", 30);
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}
}
