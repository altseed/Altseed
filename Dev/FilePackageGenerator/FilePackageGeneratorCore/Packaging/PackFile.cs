using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using IO = System.IO;
using FilePackageGenerator.Extention;

namespace FilePackageGenerator.Packaging
{
	internal class PackFile
	{
		public const string Extension = ".pack";
		public const string ExtensionWithoutDot = "pack";
		public TopHeader TopHeader
		{
			get;
			private set;
		}
		public string Name
		{
			get;
			private set;
		}
		Dictionary<string, InternalHeader> InternalHeader;

		private DateTime LastWriteTime;
		private static readonly char[] TrimEndChars;

		static PackFile()
		{
			TrimEndChars = (char[])typeof(System.IO.Path).GetField("TrimEndChars", System.Reflection.BindingFlags.DeclaredOnly | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Static).GetValue(null);
		}
		public PackFile(Uri uri, string key)
		{
			string fullPath = uri.LocalPath.RemoveLastDirectorySeparator();
			LastWriteTime = IO.File.GetLastWriteTime(fullPath);
			Name = IO.Path.GetFileName(fullPath);

			using (var reader = new IO.BinaryReader(new FileStream(uri, key)))
			{
				TopHeader = new TopHeader(reader);

				InternalHeader = new Dictionary<string, InternalHeader>((int)TopHeader.FileCount);
				for (ulong i = 0; i < TopHeader.FileCount; i++)
				{
					var tmp = new InternalHeader(reader);
					InternalHeader.Add(tmp.FileName, tmp);
				}
			}
		}

		public InternalHeader InternalFileInfo(string name)
		{
			string lowerName = name.ToLower();

			if (InternalHeader.ContainsKey(lowerName))
				return InternalHeader[lowerName];
			else
				return null;
		}
		public bool HaveFile(string name)
		{
			string lowerName = name.ToLower();

			return InternalHeader.ContainsKey(lowerName);
		}
		public IEnumerable<string> EnumerateFiles(string directoryPath)
		{
			return EnumerateFiles(directoryPath, "");
		}
		public IEnumerable<string> EnumerateFiles(string directoryPath, string searchPattern)
		{
			return EnumerateFiles(directoryPath, searchPattern, IO.SearchOption.TopDirectoryOnly);
		}
		public IEnumerable<string> EnumerateFiles(string directoryPath, string searchPattern, IO.SearchOption searchOption)
		{
			if (directoryPath == null)
				throw new ArgumentNullException("directoryPath");
			if (searchPattern == null)
				throw new ArgumentNullException("searchPattern");
			if (HaveFile(directoryPath))
				throw new IO.IOException("ファイル名は指定できません");
			if (searchPattern.Contains(IO.Path.AltDirectorySeparatorChar) || searchPattern.Contains(IO.Path.DirectorySeparatorChar))
				throw new ArgumentException("searchPattern");

			string lowerDirectoryPath = directoryPath.ToLower();
			var regex = new System.Text.RegularExpressions.Regex(searchPattern.TrimEnd(TrimEndChars).Replace("?", @".").Replace("*", @".*") + "$");

			switch (searchOption)
			{
				case System.IO.SearchOption.AllDirectories:
					return InternalHeader.Keys
						.Where(key => key.StartsWith(lowerDirectoryPath))
						.Where(key => regex.IsMatch(IO.Path.GetFileName(key)));
				case System.IO.SearchOption.TopDirectoryOnly:
					return InternalHeader.Keys
						.Where(key => key.StartsWith(lowerDirectoryPath))
						.Where(key => !key.Substring(lowerDirectoryPath.Length).Contains(IO.Path.PathSeparator))
						.Where(key => regex.IsMatch(IO.Path.GetFileName(key)));
				default:
					throw new ArgumentException("searchOption");
			}
		}
		public override string ToString()
		{
			return Name;
		}
	}
}
