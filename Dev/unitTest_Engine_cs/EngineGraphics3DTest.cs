using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs
{
	class EngineGraphics3DTest : EngineTest
	{
		public asd.Layer3D Layer3D
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
			var scene = new asd.Scene();
			var layer = new asd.Layer3D();
			Layer3D = layer;
			scene.AddLayer(layer);
			asd.Engine.ChangeScene(scene);

			var cameraObj = new asd.CameraObject3D();
			layer.AddObject(cameraObj);

			cameraObj.Position = new asd.Vector3DF(40, 20, 40);
			cameraObj.Focus = new asd.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 50.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 200.0f;
			cameraObj.WindowSize = new asd.Vector2DI(800, 600);
		}
	}
}
