using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace sample_cs
{
	/// <summary>
	/// ログを Log.html に出力するサンプル。
	/// </summary>
	class Log : ISample
	{
		public void Run()
		{
			// 初期設定を行う。
			var option = new ace.EngineOption
			{
				IsFullScreen = false
			};

			ace.Engine.Initialize( "Log", 100, 100, option);

			// Engineに格納されたものを使う（Log.htmlに出力される）
			var logger = ace.Engine.Logger;

			logger.Write( "文字列" );
			logger.WriteLine( "文字列＋改行" );
			logger.WriteLine( "<文字列>" );
			logger.WriteHeading( "ヘッダ" );
			logger.WriteLineStrongly( "強調文字列" );

			// 水平線(<hr/>)
			logger.WriteHorizontalRule();

			// 表組み
			logger.BeginTable();
			logger.Write( "セル１" );
			logger.ChangeColumn();
			logger.Write( "セル２(ChangeColumn)" );
			logger.ChangeRow();
			logger.Write( "セル３(ChangeRow)" );
			logger.EndTable();
            
			// SetOutputLevelメソッドで指定したものより低いレベルに指定した出力は、実行されない
			logger.SetOutputLevel( ace.LogLevel.Critical );
			logger.WriteLine( "出力されるログ(critical)", ace.LogLevel.Critical );
			logger.WriteLine( "出力されないログ(information)", ace.LogLevel.Information );

            logger.BeginTable( ace.LogLevel.Warning );
			logger.Write( "出力されないテーブル", ace.LogLevel.Warning );
			logger.EndTable( ace.LogLevel.Warning );

			Console.WriteLine( "Log.html に出力しました" );

			ace.Engine.Terminate();
		}
	}
}
