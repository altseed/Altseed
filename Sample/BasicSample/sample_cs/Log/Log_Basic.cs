using System;

namespace sample_cs.Log
{
	class Log_Basic : ISample
	{
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Log", 640, 480, new asd.EngineOption());

			// Engineの標準のロガーを使う（Log.htmlに出力される）
			var logger = asd.Engine.Logger;

			// ヘッダー文字列を出力する
			logger.WriteHeading("サンプル出力");

			// 文字列を出力する
			logger.Write("文字列");

			// 改行付きで文字列を出力する
			logger.WriteLine("文字列＋改行");

			// 山括弧のエスケープもされる
			logger.WriteLine("<文字列>");

			// 水平線(<hr/>)
			logger.WriteHorizontalRule();

			// 強調された文字列を出力する
			logger.WriteLineStrongly("強調文字列");


			// メッセージ用の文字列を用意する。TextObject2D_DynamicFontのサンプルを参照
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
			get { return "ログファイルにログを出力するサンプルです。"; }
		}

		public string Title
		{
			get { return "ログ出力"; }
		}
	}
}
