using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs.Log
{
	class Log_OutputLevel : ISample
	{
		public void Run()
		{
			// Altseedを初期化する。
			asd.Engine.Initialize("Log_OutputLevel", 640, 480, new asd.EngineOption());

			// Engineの標準のロガーを使う（Log.htmlに出力される）
			var logger = asd.Engine.Logger;

			// ヘッダー文字列を出力する
			logger.WriteHeading("サンプル出力");

			// SetOutputLevelメソッドで指定したものより低いレベルに指定した出力は、実行されない
			// レベルが高い方から順に、Information, Warning, Critical, Error
			logger.OutputLevel = asd.LogLevel.Critical;

			// Criticalレベルで出力する。
			logger.WriteLine("出力されるログ(critical)", asd.LogLevel.Critical);

			// Informationレベルで出力する。
			logger.WriteLine("出力されないログ(information)", asd.LogLevel.Information);


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
			get { return "ログファイルにログを出力するサンプルです。\n出力レベルを指定し、低レベルのログを無視します。"; }
		}

		public string Title
		{
			get { return "ログの出力レベルの指定"; }
		}
	}
}
