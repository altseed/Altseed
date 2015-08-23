using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace asd
{
	public partial class StaticFile : IDestroy
	{
		private byte[] buffer;

		internal StaticFile(swig.StaticFile swig)
		{

#if DEBUG
			if (GC.StaticFiles.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif

			CoreInstance = swig;
		}

		~StaticFile()
		{
			Destroy();
		}


		/// <summary>
		/// 
		/// </summary>
		public bool IsDestroyed
		{
			get
			{
				return CoreInstance == null;
			}
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public void Destroy()
		{
			lock (this)
			{
				if (CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 読み込まれたバッファを取得する。
		/// </summary>
		unsafe public byte[] Buffer
		{
			get
			{
				if (buffer == null)
				{
					System.IntPtr raw = CoreInstance.GetData();
					byte[] bytes = new byte[CoreInstance.GetSize()];
					Marshal.Copy(raw, bytes, 0, CoreInstance.GetSize());
					buffer = bytes;

					/*
					 var buf = CoreInstance.GetBuffer();
					buffer = new byte[buf.Count];
					buf.CopyTo(buffer);
					 */
				}

				return buffer;
			}
		}
	}
}
