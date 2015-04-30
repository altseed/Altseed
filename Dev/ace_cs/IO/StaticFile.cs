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

			coreInstance = swig;
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
					System.IntPtr raw = coreInstance.GetData();
					byte[] bytes = new byte[coreInstance.GetSize()];
					Marshal.Copy(raw, bytes, 0, coreInstance.GetSize());
					buffer = new List<byte>(bytes);
				}

				return buffer;
			}
		}
	}
}
