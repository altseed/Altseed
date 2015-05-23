using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ace
{
	public partial class StaticFile : IDestroy
	{
		private List<byte> buffer;

		internal StaticFile(swig.StaticFile swig)
		{

#if DEBUG
			if (GC.StaticFiles.GetObject(swig.GetPtr()) != null) throw new Exception();
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

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		unsafe public List<byte> Buffer
		{
			get
			{
				if (buffer == null)
				{
					System.IntPtr raw = CoreInstance.GetData();
					byte[] bytes = new byte[CoreInstance.GetSize()];
					Marshal.Copy(raw, bytes, 0, CoreInstance.GetSize());
					buffer = new List<byte>(bytes);
				}

				return buffer;
			}
		}
	}
}
