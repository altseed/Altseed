using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
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

		public override void Destroy()
		{
			coreObject = null;
			base.Destroy();
		}

		protected override void OnStart() { }
		protected override void OnUpdate() { }

		/// <summary>
		/// 地形を設定する。
		/// </summary>
		/// <param name="terrain">地形</param>
		public void SetTerrain(Terrain3D terrain)
		{
			coreObject.SetTerrain(IG.GetTerrain3D(terrain));
		}
	}
}
