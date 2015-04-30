using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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


	}
}
