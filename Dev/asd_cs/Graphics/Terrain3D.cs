using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 地形のクラス
	/// </summary>
	public partial class Terrain3D : IDestroy
	{
		internal Terrain3D(swig.Terrain3D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Terrain3Ds.GetObject(swig.GetPtr()) != null) Particular.Helper.ThrowException("");
#endif
			CoreInstance = swig;
		}

		~Terrain3D()
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
				if (CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			CoreInstance.SetMaterial(IG.GetMaterial3D(material));
		}

		/// <summary>
		/// メモリから地形のデータを読み込む。
		/// </summary>
		/// <param name="buffer">バッファ</param>
		/// <remarks>
		/// テクスチャのパスは保存されないので、読み込んだ後にAddSurfaceする必要がある。
		/// </remarks>
		public void LoadFromMemory(byte[] buffer)
		{
			var buf = new swig.VectorUint8();
			foreach(var b in buffer)
			{
				buf.Add(b);
			}

			CoreInstance.LoadFromMemory(buf);

			buf.Dispose();
		}

		/// <summary>
		/// メモリに地形のデータを保存する。
		/// </summary>
		/// <returns>地形のデータ</returns>
		public byte[] SaveToMemory()
		{
			var buf = CoreInstance.SaveToMemory();
			byte[] dst = new byte[buf.Count];
			buf.CopyTo(dst);

			return dst;
		}
	}
}
