using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 地形のクラス
	/// </summary>
	public class Terrain3D : IDestroy
	{
		internal swig.Terrain3D SwigObject { get; set; }

		internal Terrain3D(swig.Terrain3D swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Texture2Ds.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Terrain3D()
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
		/// 新規に地形を作成する。
		/// </summary>
		/// <param name="gridSize">グリッド1つ当たりの大きさ</param>
		/// <param name="gridWidthCount">横方向のグリッド数</param>
		/// <param name="gridHeightCount">縦方向のグリッド数</param>
		public void New(float gridSize, int gridWidthCount, int gridHeightCount)
		{
			SwigObject.New(gridSize, gridWidthCount, gridHeightCount);
		}

		/// <summary>
		/// 素材を追加する。
		/// </summary>
		/// <param name="name">素材名</param>
		/// <param name="size">素材のテクスチャ1枚あたりの大きさ</param>
		/// <param name="color">色テクスチャのパス</param>
		/// <param name="normal">法線テクスチャのパス</param>
		/// <param name="metalness">金属度テクスチャのパス</param>
		public void AddSurface(string name, float size, string color, string normal, string metalness)
		{
			SwigObject.AddSurface(name, size, color, normal, metalness);
		}

		/// <summary>
		/// 素材インデックスを取得する。
		/// </summary>
		/// <param name="name">素材名</param>
		/// <returns>素材インデックス</returns>
		public int GetSurfaceIndex(string name)
		{
			return SwigObject.GetSurfaceIndex(name);
		}

		/// <summary>
		/// 素材を円形に割り当てる。
		/// </summary>
		/// <param name="surfaceIndex">素材インデックス</param>
		/// <param name="x">円形の中心座標</param>
		/// <param name="y">円形の中心座標</param>
		/// <param name="radius">円形の半径</param>
		/// <param name="value">割り当てる値(-255～255)</param>
		/// <param name="fallout">周囲のぼかし(0～1)</param>
		public void AssignSurfaceWithCircle(int surfaceIndex, float x, float y, float radius, float value, float fallout)
		{
			SwigObject.AssignSurfaceWithCircle(surfaceIndex, x, y, radius, value, fallout);
		}

		/// <summary>
		/// 材質を設定する。
		/// </summary>
		/// <param name="material">材質</param>
		public void SetMaterial(int materialIndex, Material3D material)
		{
			SwigObject.SetMaterial(IG.GetMaterial3D(material));
		}
	}
}
