using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FilePackageGenerator.Packaging
{
	internal class TopHeader
	{
		public const char FilePathSeparator = '\t';
		IEnumerable<string> ignoreFiles;
		/// <summary>
		/// ヘッダサイズ
		/// </summary>
		public UInt32 HeaderSize
		{
			get;
			private set;
		}
		/// <summary>
		/// 無視するファイルを入れている領域サイズ
		/// </summary>
		public UInt64 FilePathHeaderLength
		{
			get;
			private set;
		}
		/// <summary>
		/// シグネチャ
		/// </summary>
		public string Signature
		{
			get;
			private set;
		}

		/// <summary>
		/// ファイル数
		/// </summary>
		public UInt64 FileCount
		{
			get;
			private set;
		}
		
		/// <summary>
		/// 読み取り用コンストラクタ
		/// </summary>
		/// <param name="reader"></param>
		public TopHeader(System.IO.BinaryReader reader)
		{
			reader.BaseStream.Seek(0, System.IO.SeekOrigin.Begin);

			Signature = Encoding.UTF8.GetString(reader.ReadBytes(4));
			if (Signature != "pack")
				throw new Exception("パックファイルのフォーマットではありません");

			FileCount = reader.ReadUInt32();
			FilePathHeaderLength = reader.ReadUInt32();
			ignoreFiles = Encoding.UTF8.GetString(reader.ReadBytes((int)FilePathHeaderLength)).Split(new char[] { FilePathSeparator }, StringSplitOptions.RemoveEmptyEntries);
			HeaderSize = (uint)(4 + 8 + 8 + FilePathHeaderLength);
		}
		/// <summary>
		/// 書き込み用コンストラクタ
		/// </summary>
		/// <param name="fileCount"></param>
		/// <param name="ignoreFile"></param>
		public TopHeader(uint fileCount, IEnumerable<string> ignoreFile)
		{
			if (ignoreFile == null)
				ignoreFile = new string[0];

			Signature = "pack";
			FileCount = fileCount;
			ignoreFiles = ignoreFile.Select(file=>file.ToLower());
			foreach (string path in ignoreFiles)
			{
				FilePathHeaderLength += (uint)(path + FilePathSeparator).Length;
			}
			HeaderSize = (uint)(4 + 8 + 8 + FilePathHeaderLength);
		}

		public byte[] ToByteArray()
		{
			List<byte> tmp = new List<byte>((int)HeaderSize);

			tmp.AddRange(Encoding.UTF8.GetBytes(Signature));
			tmp.AddRange(BitConverter.GetBytes(FileCount));
			tmp.AddRange(BitConverter.GetBytes(FilePathHeaderLength));
			foreach (string ignorePath in ignoreFiles)
			{
				tmp.AddRange(Encoding.UTF8.GetBytes(ignorePath + FilePathSeparator));
			}

			return tmp.ToArray();
		}
		public string[] GetIgnorefiles()
		{
			return ignoreFiles.ToArray();
		}
	}
	internal class InternalHeader
	{
		public uint HeaderSize
		{
			get;
			private set;
		}

		public uint Size
		{
			get;
			private set;
		}

		public uint Offset
		{
			get;
			internal set;
		}

		public string FileName
		{
			get;
			private set;
		}
		public uint FileNameLength
		{
			get;
			private set;
		}
		/// <summary>
		/// 読み込み用コンストラクタ
		/// </summary>
		/// <param name="reader"></param>
		public InternalHeader(System.IO.BinaryReader reader)
		{
			FileNameLength = reader.ReadUInt32();
			Size = reader.ReadUInt32();
			Offset = reader.ReadUInt32();
			FileName = Encoding.UTF8.GetString(reader.ReadBytes((int)FileNameLength));

			HeaderSize = (uint)(4 + 4 + 4 + FileNameLength);
		}
		/// <summary>
		/// 書き込み用コンストラクタ
		/// </summary>
		/// <param name="preCompressedSize"></param>
		/// <param name="packedSize"></param>
		/// <param name="offset"></param>
		/// <param name="fileName"></param>
		public InternalHeader(uint size, uint offset, string fileName)
		{
			FileNameLength = (byte)Encoding.UTF8.GetByteCount(fileName);
			Size = size;
			Offset = offset;
			FileName = fileName.ToLower();

			HeaderSize = (uint)(4 + 4 + 4 + FileNameLength);
		}

		public byte[] ToByteArray()
		{
			List<byte> tmp = new List<byte>((int)HeaderSize);

			tmp.AddRange(BitConverter.GetBytes(FileNameLength));
			tmp.AddRange(BitConverter.GetBytes(Size));
			tmp.AddRange(BitConverter.GetBytes(Offset));
			tmp.AddRange(Encoding.UTF8.GetBytes(FileName));

			return tmp.ToArray();
		}
	}
}
