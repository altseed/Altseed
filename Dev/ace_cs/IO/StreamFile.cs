using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class StreamFile : IDestroy
	{
		internal swig.StreamFile SwigObject { get; set; }

		internal StreamFile(swig.StreamFile swig)
		{

#if DEBUG
			if (GC.StreamFiles.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif

			SwigObject = swig;
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


	}
}
