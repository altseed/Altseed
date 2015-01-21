using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._3D
{
	class TerrainObject3D : EngineGraphics3DTest
	{
		public TerrainObject3D()
			: base(10)
		{

		}

		protected override void OnStart()
		{
			base.OnStart();

			var lightObj = new ace.DirectionalLightObject3D();
			lightObj.Rotation = new ace.Vector3DF(30, 160, 0);
			Layer3D.AddObject(lightObj);


			var t = ace.Engine.Graphics.CreateTerrain3D();
			t.New(1, 16, 16);
			t.AddSurface(
			"sf1",
			2,
			"Data/Texture/Surface/Tile_Color.png",
			"Data/Texture/Surface/Tile_Normal.png",
			"Data/Texture/Surface/Tile_Spec.png");

			t.AddSurface(
				"sf2",
				2,
				"Data/Texture/Surface/Cloud_Color.png",
				"Data/Texture/Surface/Cloud_Normal.png",
				"Data/Texture/Surface/Cloud_Spec.png");

			var sf2ind = t.GetSurfaceIndex("sf2");
			t.AssignSurfaceWithCircle(sf2ind, 0, 0, 2, 255, 0.5f);

			var tObj = new ace.TerrainObject3D();
			tObj.SetTerrain(t);
			Layer3D.AddObject(tObj);
		}
	}
}
