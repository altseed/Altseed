using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace FilePackingTool.Extention
{
	internal static class PathStringExtention
	{
		/// <summary>
		/// 文字列の最後にあるディレクトリ区切り記号を削除します
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static string RemoveLastDirectorySeparator(this string path)
		{
			if (path.IsLastDirectorySeparatorChar())
				return path.Substring(0, path.Length - 1);
			else
				return path;
		}
		/// <summary>
		/// 文字列の最後にディレクトリ区切り記号を必要なら付けます
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static string AddLastDirectorySeparator(this string path)
		{
			if (path.IsLastDirectorySeparatorChar())
				return path;
			else
				return path + Path.DirectorySeparatorChar;
		}
		/// <summary>
		/// 最後がディレクトリ区切り記号か
		/// </summary>
		/// <param name="path"></param>
		/// <returns></returns>
		public static bool IsLastDirectorySeparatorChar(this string path)
		{
			if (String.IsNullOrEmpty(path))
				return false;

			if (path[path.Length - 1] == Path.DirectorySeparatorChar ||
				path[path.Length - 1] == Path.AltDirectorySeparatorChar)
			{
				return true;
			}
			return false;
		}
	}
}
