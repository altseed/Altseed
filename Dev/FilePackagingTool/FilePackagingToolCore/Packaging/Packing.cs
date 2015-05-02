using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Text.RegularExpressions;
using FilePackingTool.Extention;

namespace FilePackingTool.Packaging
{
	public enum PackagingSetting
	{
		IgnoreHiddenAttribute,
		AllFiles
	}
	public class Packing
	{
		/// <summary>
		/// 隠し属性のついていないファイルと、隠し属性のついたディレクトリ下のファイル以外を列挙します
		/// </summary>
		/// <param name="directoryUri"></param>
		/// <returns></returns>
		static IEnumerable<Uri> EnumerateFilesWithoutHiddenAttribute(Uri directoryUri)
		{
			foreach (string filePath in Directory.EnumerateFiles(directoryUri.LocalPath, "*", SearchOption.TopDirectoryOnly)
				.Where(path => (File.GetAttributes(path) & FileAttributes.Hidden) == 0 && Path.GetFileName(path)[0] != '.'))
			{
				yield return new Uri(filePath);
			}
			foreach (string dirPath in Directory.EnumerateDirectories(directoryUri.LocalPath, "*", SearchOption.TopDirectoryOnly)
				.Where(path => (File.GetAttributes(path) & FileAttributes.Hidden) == 0 && Path.GetFileName(path)[0] != '.'))
			{
				foreach (var file in EnumerateFilesWithoutHiddenAttribute(new Uri(dirPath)))
				{
					yield return file;
				}
			}
		}
		static IEnumerable<Uri> EnumerateAllFiles(Uri directoryUri, PackagingSetting setting)
		{
			switch (setting)
			{
				case PackagingSetting.AllFiles:
					foreach (string filePath in Directory.EnumerateFiles(directoryUri.LocalPath, "*", SearchOption.AllDirectories))
					{
						yield return new Uri(filePath);
					}
					break;
				case PackagingSetting.IgnoreHiddenAttribute:
					foreach (var uri in EnumerateFilesWithoutHiddenAttribute(directoryUri))
					{
						yield return uri;
					}
					break;
				default:
					throw new ArgumentException("setting が無効の値を指しています");
			}
		}
		static void ErrorCheck(Uri directoryUri)
		{
			if (!Directory.Exists(directoryUri.LocalPath))
				throw new DirectoryNotFoundException("指定パスのディレクトリが見つかりませんでした");
		}
		[System.Diagnostics.Conditional("DEBUG")]
		static void PrintHeaderInfo(TopHeader topHeader, IEnumerable<InternalHeader> inHeader, string packFileName)
		{
			using (var writer = new StreamWriter(packFileName + "Debug.csv"))
			{
				writer.WriteLine("top\n");

				writer.Write("HeaderSize," + topHeader.HeaderSize + '\n');
				writer.Write("FilePathHeaderLength," + topHeader.FilePathHeaderLength + '\n');
				writer.Write("Signature," + topHeader.Signature + '\n');
				writer.Write("FileCount," + topHeader.FileCount + '\n');

				writer.WriteLine("internal\n");

				foreach (var header in inHeader)
				{
					writer.Write("HeaderSize," + header.HeaderSize + '\n');
					writer.Write("Size," + header.Size + '\n');
					writer.Write("Offset," + header.Offset + '\n');
					writer.Write("FileName," + header.FileName + '\n');
					writer.Write("FileNameLength," + header.FileNameLength + '\n');

					writer.WriteLine();
				}
			}
		}
		static unsafe void Encrypt(string path, string key)
		{
			byte[] byteKey = Encoding.UTF8.GetBytes(key);

			using (var reader = File.OpenRead(path))
			using (var writer = new BinaryWriter(File.Create(path)))
			{
				byte[] array = new byte[2 * 1024];

				fixed (byte* pfixKey = byteKey)
				{
					byte* pKey = pfixKey;
					byte* pKeyEnd = pfixKey + byteKey.Length;

					for (var cursor = reader.Read(array, 0, array.Length);
						cursor == 0;
						cursor = reader.Read(array, 0, array.Length))
					{
						if (cursor == array.Length)
						{
							for (int i = 0; i < array.Length; i++)
							{
								array[i] ^= *pKey;

								pKey++;
								if (pKey == pKeyEnd)
								{
									pKey = pfixKey;
								}
							}
						}
						else
						{
							for (int i = 0; i < cursor; i++)
							{
								array[i] ^= *pKey;

								pKey++;
								if (pKey == pKeyEnd)
								{
									pKey = pfixKey;
								}
							}
						}

						writer.Write(array, 0, cursor);
					}
				}
			}
		}
		public static void Run(string targetPath, string packName, PackagingSetting setting, IEnumerable<string> ignorePath, string key = "")
		{
			Uri directoryUri = new Uri(Path.GetFullPath(targetPath).RemoveLastDirectorySeparator());

			if (Path.GetExtension(packName) != PackFile.Extension)
			{
				packName += PackFile.Extension;
			}


			ErrorCheck(directoryUri);

			var internalFormat = new List<InternalHeader>();

			uint offset = 0;
			uint headerSize = 0;

			foreach (Uri uri in EnumerateAllFiles(directoryUri, setting))
			{
				uint size = 0;
				using (var stream = File.Open(uri.LocalPath, FileMode.Open))
				{
					size = (uint)stream.Length;
				}

				InternalHeader tmp = new InternalHeader(size,
														offset,
														// *.pack/hoge/fuga の *.pack/ を削除
														new string(directoryUri.MakeRelativeUri(uri).ToString()
																	.SkipWhile(ch => ch != Path.AltDirectorySeparatorChar && ch != Path.DirectorySeparatorChar)
																	.Skip(1)
																	.ToArray()));
				internalFormat.Add(tmp);
				headerSize += tmp.HeaderSize;
				offset += size;
			}
			TopHeader format = new TopHeader((uint)internalFormat.Count, ignorePath
				.Where(path => !String.IsNullOrEmpty(path))
				.Select(path => directoryUri.MakeRelativeUri(new Uri(Path.GetFullPath(path))).ToString()));

			internalFormat.ForEach(inHeader => inHeader.Offset += headerSize + format.HeaderSize);

			PrintHeaderInfo(format, internalFormat, packName);

			string exportedPath = string.Empty;
			if (System.IO.Path.IsPathRooted(packName))
			{
				exportedPath = packName;
			}
			else
			{
				exportedPath = Path.GetDirectoryName(directoryUri.LocalPath)
					// 実行ファイルと同じ階層に生成したい場合は GetFileName に変更
					+ Path.DirectorySeparatorChar
					+ packName;
			}

			using (var writer = new BinaryWriter(File.Create(exportedPath)))
			{
				writer.Write(format.ToByteArray());
				internalFormat.ForEach(inHeader => writer.Write(inHeader.ToByteArray()));

				foreach (string fullPath in EnumerateAllFiles(directoryUri, setting)
					.Select(path => new Uri(directoryUri, path).LocalPath))
				{
					writer.Write(File.ReadAllBytes(fullPath));
				}
			}

			if (!string.IsNullOrEmpty(key))
			{
				Encrypt(exportedPath, key);
			}
		}
	}
}