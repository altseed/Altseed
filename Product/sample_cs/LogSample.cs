using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace test_cs
{
	/// <summary>
	/// ログを Log.html に出力するサンプル。
	/// </summary>
	class LogSample : ISample
	{
		public void Run()
		{
			ace.Engine.Initialize( "Log", 100, 100, false );

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
			logger.SetOutputLevel( ace.Log.OutputLevel.Critical );
			logger.WriteLine( "出力されるログ(critical)", ace.Log.OutputLevel.Critical );
			logger.WriteLine( "出力されないログ(information)", ace.Log.OutputLevel.Information );

			logger.BeginTable( ace.Log.OutputLevel.Warning );
			logger.Write( "出力されないテーブル", ace.Log.OutputLevel.Warning );
			logger.EndTable( ace.Log.OutputLevel.Warning );

			Console.WriteLine( "Log.html に出力しました" );

			ace.Engine.Terminate();
		}
	}
}
