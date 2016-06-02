using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Shader2D : IReleasable
	{

		internal Shader2D(swig.Shader2D instance)
		{
			CoreInstance = instance;
		}

		~Shader2D()
		{
			ForceToRelease();
		}

		public bool IsReleased
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
		public void ForceToRelease()
		{
			lock (this)
			{
				if (CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
	}
}
