using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/*
	/// <summary>
	/// ログの出力レベルを表す列挙体。レベルがLogクラスに設定されたものより高いログのみが記録されます。
	/// </summary>
	public enum LogLevel
	{
		/// <summary>
		/// プログラムを停止しなければならない状況に関するログの出力レベル。
		/// </summary>
		Error = swig.LogLevel.Error,
		/// <summary>
		/// プログラムが正常に動作できないことに関するログの出力レベル。
		/// </summary>
		Critical = swig.LogLevel.Critical,
		/// <summary>
		/// プログラムが正常に動作することが保証できない状況に関するログの出力レベル。
		/// </summary>
		Warning = swig.LogLevel.Warning,
		/// <summary>
		/// プログラムの動作の異常ではなく、単に動作の状況を示すログの出力レベル。
		/// </summary>
		Information = swig.LogLevel.Information,
		/// <summary>
		/// 最低の出力レベル。
		/// </summary>
		All = swig.LogLevel.All,
	}
	*/

	/// <summary>
	/// テキスト形式でログを記録し、HTMLファイルに保存する機能を提供するクラス。
	/// </summary>
	public partial class Log
	{
		/// <summary>
		/// Logクラスをインスタンス化します。
		/// </summary>
		/// <param name="log">ネイティブのラッパー</param>
		internal Log(swig.Log log)
		{
			CoreInstance = log;
		}
	}
}
