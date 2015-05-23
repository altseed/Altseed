using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public partial class Texture2D : Texture, IDestroy
	{
		internal swig.Texture2D SwigObject
		{
			get { return CoreInstance; }
			set { CoreInstance = value; }
		}

		internal Texture2D(swig.Texture2D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if(GC.Texture2Ds.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			CoreInstance = swig;
		}

		~Texture2D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return CoreInstance == null;
			}
		}

		public void Destroy()
		{
			lock (this)
			{
				if(CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// テクスチャをロックし編集可能にする。
		/// </summary>
		/// <param name="info">テクスチャ情報</param>
		/// <returns>成否</returns>
		public bool Lock(TextureLockInfomation info)
		{
			if(info == null) return false;

			return CoreInstance.Lock(info.SwigObject);
		}
	}
}
