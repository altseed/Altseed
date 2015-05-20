using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace FilePackingTool.Extention
{
	internal static class UriExtention
	{
		static bool Contains(string baseFullPath, string valueFullPath)
		{
			if (baseFullPath.Length < valueFullPath.Length)
				return false;


			for (int i = 0; i < valueFullPath.Length; i++)
			{
				if (baseFullPath[i] != valueFullPath[i])
					return false;
			}
			return true;
		}
		/// <summary>
		/// 引数で指定した uri の先祖にあたるか
		/// </summary>
		/// <param name="base"></param>
		/// <param name="target"></param>
		/// <returns></returns>
		public static bool IsAncestor(this Uri @base, Uri target)
		{
			string baseStr = @base.LocalPath.ToLower().RemoveLastDirectorySeparator();
			string targetStr = target.LocalPath.ToLower().RemoveLastDirectorySeparator();

			if (baseStr == targetStr)
				return false;

			if (Contains(targetStr, baseStr))
				return true;
			else
				return false;
		}
		public static bool IsEqualOrAncestor(this Uri @base, Uri target)
		{
			string baseStr = @base.LocalPath.ToLower().RemoveLastDirectorySeparator();
			string targetStr = target.LocalPath.ToLower().RemoveLastDirectorySeparator();

			if (Contains(targetStr, baseStr))
				return true;
			else
				return false;
		}
		/// <summary>
		/// 引数で指定した uri の子孫にあたるか
		/// </summary>
		/// <param name="base"></param>
		/// <param name="target"></param>
		/// <returns></returns>
		public static bool IsOffstring(this Uri @base, Uri target)
		{
			string baseStr = @base.LocalPath.ToLower().RemoveLastDirectorySeparator();
			string targetStr = target.LocalPath.ToLower().RemoveLastDirectorySeparator();

			if (baseStr == targetStr)
				return false;

			if (Contains(targetStr, baseStr))
				return true;
			else
				return false;
		}
		public static bool IsEqualOrOffstring(this Uri @base, Uri target)
		{
			string baseStr = @base.LocalPath.ToLower().RemoveLastDirectorySeparator();
			string targetStr = target.LocalPath.ToLower().RemoveLastDirectorySeparator();

			if (Contains(targetStr, baseStr))
				return true;
			else
				return false;
		}
	}
}
