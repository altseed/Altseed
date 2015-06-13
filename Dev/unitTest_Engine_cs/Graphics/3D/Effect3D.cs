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
			var scene = new asd.Scene();
			var layer = new asd.Layer3D();
			var effectObj = new asd.EffectObject3D();
			var lightObj = new asd.DirectionalLightObject3D();
			var cameraObj = new asd.CameraObject3D();
			var effect = asd.Engine.Graphics.CreateEffect("Data/Effect/Laser01.efk");
			scene.AddLayer(layer);
			layer.AddObject(effectObj);
			layer.AddObject(lightObj);
			layer.AddObject(cameraObj);
			asd.Engine.ChangeScene(scene);

			cameraObj.Position = new asd.Vector3DF(40, 20, 40);
			cameraObj.Focus = new asd.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 50.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 200.0f;
			cameraObj.WindowSize = new asd.Vector2DI(800, 600);

			effectObj.Rotation = new asd.Vector3DF(0.0f, 20.0f, 0.0f);

			lightObj.Rotation = new asd.Vector3DF(30, 160, 0);

			effectObj.SetEffect(effect);
			effectObj.Play();
		}
	}
}
