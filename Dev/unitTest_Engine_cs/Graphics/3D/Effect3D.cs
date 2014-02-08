using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._3D
{
	class Effect3D : EngineTest
	{
		public Effect3D()
			: base(20)
		{
		}

		protected override void OnStart()
		{
			var scene = new ace.Scene();
			var layer = new ace.Layer3D();
			var effectObj = new ace.EffectObject3D();
			var lightObj = new ace.DirectionalLightObject3D();
			var cameraObj = new ace.CameraObject3D();
			var effect = ace.Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk");
			scene.AddLayer(layer);
			layer.AddObject(effectObj);
			layer.AddObject(lightObj);
			layer.AddObject(cameraObj);
			ace.Engine.ChangeScene(scene);

			cameraObj.Position = new ace.Vector3DF(40, 20, 40);
			cameraObj.Focus = new ace.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 50.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 200.0f;
			cameraObj.WindowSize = new ace.Vector2DI(800, 600);

			effectObj.Rotation = new ace.Vector3DF(0.0f, 20.0f, 0.0f);

			lightObj.Rotation = new ace.Vector3DF(30, 160, 0);

			effectObj.SetEffect(effect);
			effectObj.Play();
		}
	}
}
