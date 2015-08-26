using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Texture2D : Texture, IReleasable
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
			if (GC.Texture2Ds.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			CoreInstance = swig;
		}

		~Texture2D()
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
