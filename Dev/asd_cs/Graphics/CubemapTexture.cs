using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// キューブマップ
	/// </summary>
	public class CubemapTexture : Texture, IDestroy
	{
		internal swig.CubemapTexture SwigObject { get; set; }

		internal CubemapTexture(swig.CubemapTexture swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.CubemapTextures.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			SwigObject = swig;
		}

		~CubemapTexture()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
 			get
			{
				return SwigObject == null;
			}
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public void Destroy()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// ミップマップ数
		/// </summary>
		public int MipmapCount
		{
			get
			{
				return SwigObject.GetMipmapCount();
			}
		}
	}
}
