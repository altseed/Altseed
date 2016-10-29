using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 文字列に関する補助を行うクラス
	/// </summary>
	public class StringHelper
	{
		/// <summary>
		/// UTF8のデータを文字列に変換する。
		/// </summary>
		/// <param name="buffer"></param>
		/// <returns></returns>
		public static string ConvertUTF8(byte[] buffer)
		{
			return Particular.Helper.ConvertUTF8(buffer);
		}

		/// <summary>
		/// UTF8のデータを文字列に変換する。
		/// </summary>
		/// <param name="buffer"></param>
		/// <returns></returns>
		public static string ConvertUTF8(List<byte> buffer)
		{
			return Particular.Helper.ConvertUTF8(buffer);
		}
	}
}
