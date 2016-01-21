using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 地形を描画するクラス
	/// </summary>
	public class TerrainObject3D : Object3D
	{
		swig.CoreTerrainObject3D coreObject = null;

		public TerrainObject3D()
		{
			coreObject = Engine.ObjectSystemFactory.CreateTerrainObject3D();
			GC.Object3Ds.AddObject(coreObject.GetPtr(), this);
			commonObject = coreObject;
		}

		public override void ForceToRelease()
		{
			coreObject = null;
			base.ForceToRelease();
		}

		protected override void OnAdded() { }
		protected override void OnUpdate() { }

		/// <summary>
		/// 地形を設定する。
		/// </summary>
		/// <param name="terrain">地形</param>
		public void SetTerrain(Terrain3D terrain)
		{
			coreObject.SetTerrain(IG.GetTerrain3D(terrain));
		}

		/// <summary>
		/// 材質個別に描画時のパラメーターを設定する。
		/// </summary>
		/// <param name="block">プロパティ</param>
		/// <remarks>
		/// 通常描画する場合、材質に設定されたパラメーターを元に描画するがモデルごとに個別のパラメーターを使用する場合、この関数を使用する。
		/// </remarks>
		public void SetMaterialPropertyBlock(MaterialPropertyBlock block)
		{
			coreObject.SetMaterialPropertyBlock(IG.GetMaterialPropertyBlock(block));
		}
	}
}
