
class Log_Table : ISample
{
	public void Run()
	{
		// Altseedを初期化する。
		asd.Engine.Initialize("Log", 640, 480, new asd.EngineOption());

		// Engineの標準のロガーを使う（Log.htmlに出力される）
		var logger = asd.Engine.Logger;

		// ヘッダーを出力する
		logger.WriteHeading("サンプル出力");

		// 表組みでの出力を開始する
		logger.BeginTable();

		// 表のセルに文字列を出力する
		logger.Write("セル１");

		// 次の列に進む
		logger.ChangeColumn();

		// 表のセルに文字列を出力する
		logger.Write("セル２(ChangeColumn)");

		// 次の行に進む。前の行にはもう戻れない
		logger.ChangeRow();

		// 表のセルに文字列を出力する
		logger.Write("セル３(ChangeRow)");

		// 表組みでの出力を終了する
		logger.EndTable();


		// SetOutputLevelメソッドで指定したものより低いレベルに指定した出力は、実行されない
		// レベルが高い方から順に、Information, Warning, Critical, Error
		logger.OutputLevel = asd.LogLevel.Critical;

		// Warningレベルで表組みで出力する。Criticalより低いレベルなので出力されない
		logger.BeginTable(asd.LogLevel.Warning);
		logger.Write("出力されないテーブル", asd.LogLevel.Warning);
		logger.EndTable(asd.LogLevel.Warning);


		// メッセージ文字列を用意する。TextObject_DynamicFontのサンプルを参照
		var font = asd.Engine.Graphics.CreateDynamicFont("", 26, new asd.Color(255, 255, 255, 255), 1, new asd.Color(0, 0, 0, 255));
		var obj = new asd.TextObject2D();
		obj.Font = font;
		obj.Position = new asd.Vector2DF(0, 0);
		obj.Text = "Log.htmlにログを出力しました。\nウィンドウを閉じる前に確認してください。";
		asd.Engine.AddObject2D(obj);

		// Altseedのウインドウが閉じられていないか確認する。
		while(asd.Engine.DoEvents())
		{
			// Altseedを更新する。
			asd.Engine.Update();
		}

		// Altseedを終了する。
		asd.Engine.Terminate();
	}

	public string Description
	{
		get { return "ログファイルに表組みで表現されたログを出力するサンプルです。"; }
	}

	public string Title
	{
		get { return "表組みされたログ出力"; }
	}

	public string ClassName
	{
		get { return "Log_Table"; }
	}
}
