using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3Dメッシュのクラス
	/// </summary>
	public partial class Mesh : IDestroy
	{
		internal ace.swig.Mesh CoreInstance { get { return coreInstance; } }

		internal Mesh(swig.Mesh swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			coreInstance = swig;
		}

		~Mesh()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get
			{
				return coreInstance == null;
			}
		}

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
		/// 内部シェーダーを使用する場合のカラーテクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetColorTexture(int materialIndex, Texture2D texture)
		{
			coreInstance.SetColorTexture(materialIndex, IG.GetTexture2D(texture));
		}

		/// <summary>
		/// 内部シェーダーを使用する場合の法線テクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetNormalTexture(int materialIndex, Texture2D texture)
		{
			coreInstance.SetNormalTexture(materialIndex, IG.GetTexture2D(texture));
		}

		/// <summary>
		/// 内部シェーダーを使用する場合の金属度テクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetMetalnessTexture(int materialIndex, Texture2D texture)
		{
			coreInstance.SetMetalnessTexture(materialIndex, IG.GetTexture2D(texture));
		}

		/// <summary>
		/// 内部シェーダーを使用する場合の面平滑度テクスチャを設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="texture">テクスチャ</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetSmoothnessTexture(int materialIndex, Texture2D texture)
		{
			coreInstance.SetSmoothnessTexture(materialIndex, IG.GetTexture2D(texture));
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="materialIndex">材質のインデックス</param>
		/// <param name="material">材質</param>
		/// <remarks>AddMaterialCountを実行した後でないと無効になる。</remarks>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			coreInstance.SetMaterial(materialIndex, IG.GetMaterial3D(material));
		}
	}
}
