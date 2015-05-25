using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace ace
{
	public partial class StreamFile : IDestroy
	{
		internal StreamFile(swig.StreamFile swig)
		{

#if DEBUG
			if (GC.StreamFiles.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif

			CoreInstance = swig;
		}

		~StreamFile()
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
		/// 
		/// </summary>
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

		public unsafe void Read(List<byte> buffer, int size)
		{
			if (buffer == null) return;

			swig.Accessor.StreamFile_Read_(CoreInstance, size);
			System.IntPtr raw = swig.Accessor.StreamFile_GetTempBuffer_(CoreInstance);
			byte[] bytes = new byte[swig.Accessor.StreamFile_GetTempBufferSize_(CoreInstance)];
			Marshal.Copy(raw, bytes, 0, bytes.Length);

			buffer.Clear();
			buffer.AddRange(bytes);
		}
	}
}
