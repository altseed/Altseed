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
			if (GC.StreamFiles.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif

			coreInstance = swig;
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
				return coreInstance == null;
			}
		}

		/// <summary>
		/// 
		/// </summary>
		public void Destroy()
		{
			lock (this)
			{
				if (coreInstance == null) return;
				GC.Collector.AddObject(coreInstance);
				coreInstance = null;
			}
			System.GC.SuppressFinalize(this);
		}

		public unsafe void Read(List<byte> buffer, int size)
		{
			if (buffer == null) return;

			swig.Accessor.StreamFile_Read_(coreInstance, size);
			System.IntPtr raw = swig.Accessor.StreamFile_GetTempBuffer_(coreInstance);
			byte[] bytes = new byte[swig.Accessor.StreamFile_GetTempBufferSize_(coreInstance)];
			Marshal.Copy(raw, bytes, 0, bytes.Length);

			buffer.Clear();
			buffer.AddRange(bytes);
		}
	}
}
