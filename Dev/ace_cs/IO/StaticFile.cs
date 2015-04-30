using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace ace
{
	public class StaticFile : IDestroy
	{
		internal swig.StaticFile SwigObject { get; set; }

		private List<byte> buffer;

		internal StaticFile(swig.StaticFile swig)
		{

#if DEBUG
			if (GC.StaticFiles.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif

			SwigObject = swig;
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
				return SwigObject == null;
			}
		}

		/// <summary>
		/// 
		/// </summary>
		public void Destroy()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
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
					System.IntPtr raw = SwigObject.GetData();
					byte[] bytes = new byte[SwigObject.GetSize()];
					Marshal.Copy(raw, bytes, 0, SwigObject.GetSize());
					buffer = new List<byte>(bytes);
				}

				return buffer;
			}
		}
	}
}
