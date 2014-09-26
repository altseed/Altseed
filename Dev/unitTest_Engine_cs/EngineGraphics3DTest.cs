using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs
{
	class EngineGraphics3DTest : EngineTest
	{
		public ace.Layer3D Layer3D
		{
			get;
			set;
		}

		public EngineGraphics3DTest(int time)
			: base(time)
		{

		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer3D();
			Layer3D = layer;
			scene.AddLayer(layer);
			ace.Engine.ChangeScene(scene);

			var cameraObj = new ace.CameraObject3D();
			layer.AddObject(cameraObj);

			cameraObj.Position = new ace.Vector3DF(40, 20, 40);
			cameraObj.Focus = new ace.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 50.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 200.0f;
			cameraObj.WindowSize = new ace.Vector2DI(800, 600);
		}
	}
}
