using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using FilePackageGenerator.Extention;

namespace FilePackageGenerator
{
	/// <summary>
	/// パックファイルも読み込めるように拡張したもの
	/// </summary>
	internal class FileStream : System.IO.FileStream
	{
		long length;
		long baseOffset;

		byte[] key;

		public override long Position
		{
			get
			{
				return base.Position - baseOffset;
			}
			set
			{
				base.Position = value;
			}
		}
		public override long Length
		{
			get
			{
				return length;
			}
		}
		public FileStream(Uri uri)
			: this(uri, "")
		{
		}
		public FileStream(Uri uri, string key)
			: base(uri.LocalPath.RemoveLastDirectorySeparator(), FileMode.Open, FileAccess.Read, FileShare.ReadWrite)
		{
			this.length = base.Length;
			this.baseOffset = 0;

			this.key = Encoding.UTF8.GetBytes(key);
		}
		public FileStream(Uri uri, long baseOffset, long length)
			: this(uri, baseOffset, length, "")
		{
		}
		public FileStream(Uri uri, long baseOffset, long length, string key)
			: base(uri.LocalPath.RemoveLastDirectorySeparator(), FileMode.Open, FileAccess.Read, FileShare.ReadWrite)
		{
			this.length = length;
			this.baseOffset = baseOffset;
			base.Seek(baseOffset, SeekOrigin.Begin);

			var key_temp = Encoding.UTF8.GetBytes(key);

			List<byte> key_ = new List<byte>();

			for (int loop = 0; loop < 40; loop++ )
			{
				for (int i = 0; i < key_temp.Count(); i++)
				{
					var k = (int)key_temp[i];
					k = (k + (loop + i)) % 255;
					key_.Add((byte)k);
				}
			}

			this.key = key_.ToArray();
		}
		public override IAsyncResult BeginRead(byte[] array, int offset, int numBytes, AsyncCallback userCallback, object stateObject)
		{
			throw new NotImplementedException("未実装");
		}
		public override int EndRead(IAsyncResult asyncResult)
		{
			throw new NotImplementedException("未実装");
		}
		unsafe void Decrypt(byte[] src, long position, int offset, int count)
		{
			fixed (byte* pfixSrc = src, pfixKey = key)
			{
				byte* pSrc = pfixSrc;
				byte* pKey = pfixKey;
				byte* pKeyEnd = pfixKey + key.Length;

				pKey = pfixKey + (offset + position) % key.Length;
				for (int i = 0; i < count; i++)
				{
					*pSrc ^= *pKey;

					pSrc++;
					if (++pKey == pKeyEnd)
						pKey = pfixKey;
				}
			}
		}
		public override int Read(byte[] array, int offset, int count)
		{
			long pos = base.Position;
			count = Position + offset + count < Length ? count : (int)(Length - Position - offset);
			int value = base.Read(array, offset, count);
			
			if (key.Length != 0)
				Decrypt(array, pos, offset, value);

			return value;
		}
		public override int ReadByte()
		{
			byte[] buf = new byte[1];

			if (Read(buf, 0, 1) != 0)
				return buf[0];
			else
				return -1;
		}
		public override long Seek(long offset, SeekOrigin origin)
		{
			switch (origin)
			{
				case SeekOrigin.Begin:
					return base.Seek(offset + baseOffset, origin);
				case SeekOrigin.Current:
					return base.Seek(offset, origin);
				case SeekOrigin.End:
					return base.Seek(offset + baseOffset + length, origin);
				default:
					return base.Seek(offset, origin);
			}
		}
	}
}