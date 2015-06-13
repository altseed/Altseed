using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Graphics._3D
{
	class SimpleMesh : EngineTest
	{
		public SimpleMesh()
			: base(10)
		{
		}

		protected override void OnStart()
		{
			var scene = new asd.Scene();
			var layer = new asd.Layer3D();
			var meshObj = new asd.ModelObject3D();
			var lightObj = new asd.DirectionalLightObject3D();
			var cameraObj = new asd.CameraObject3D();

			scene.AddLayer(layer);
			layer.AddObject(meshObj);
			layer.AddObject(lightObj);
			layer.AddObject(cameraObj);
			asd.Engine.ChangeScene(scene);

			var mesh = asd.Engine.Graphics.CreateMesh();
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, 0.5f), new asd.Vector3DF(0, 0, 1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, 0.5f), new asd.Vector3DF(0, 0, 1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, 0.5f), new asd.Vector3DF(0, 0, 1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, 0.5f), new asd.Vector3DF(0, 0, 1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, -0.5f), new asd.Vector3DF(1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, -0.5f), new asd.Vector3DF(1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, -0.5f), new asd.Vector3DF(0, 0, -1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, -0.5f), new asd.Vector3DF(0, 0, -1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, 0.5f), new asd.Vector3DF(-1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, 0.5f), new asd.Vector3DF(0, 1, 0), new asd.Vector3DF(0, 0, -1), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, 0.5f), new asd.Vector3DF(-1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, 0.5f), new asd.Vector3DF(0, -1, 0), new asd.Vector3DF(0, 0, 1), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, 0.5f), new asd.Vector3DF(1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, 0.5f), new asd.Vector3DF(0, 1, 0), new asd.Vector3DF(0, 0, -1), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, 0.5f), new asd.Vector3DF(1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, 0.5f), new asd.Vector3DF(0, -1, 0), new asd.Vector3DF(0, 0, 1), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, -0.5f), new asd.Vector3DF(0, 0, -1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, 0.5f, -0.5f), new asd.Vector3DF(0, 1, 0), new asd.Vector3DF(0, 0, -1), new asd.Vector2DF(1, 0), new asd.Vector2DF(1, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, -0.5f), new asd.Vector3DF(0, 0, -1), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(0.5f, -0.5f, -0.5f), new asd.Vector3DF(0, -1, 0), new asd.Vector3DF(0, 0, 1), new asd.Vector2DF(1, 1), new asd.Vector2DF(1, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, -0.5f), new asd.Vector3DF(-1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, 0.5f, -0.5f), new asd.Vector3DF(0, 1, 0), new asd.Vector3DF(0, 0, -1), new asd.Vector2DF(0, 0), new asd.Vector2DF(0, 0), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, -0.5f), new asd.Vector3DF(-1, 0, 0), new asd.Vector3DF(0, 1, 0), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new asd.Vector3DF(-0.5f, -0.5f, -0.5f), new asd.Vector3DF(0, -1, 0), new asd.Vector3DF(0, 0, 1), new asd.Vector2DF(0, 1), new asd.Vector2DF(0, 1), new asd.Color(255, 255, 255, 255), 0, 0);

			mesh.AddFace(0, 2, 3, -1);
			mesh.AddFace(0, 3, 1, -1);
			mesh.AddFace(12, 4, 5, -1);
			mesh.AddFace(12, 5, 14, -1);
			mesh.AddFace(16, 6, 7, -1);
			mesh.AddFace(16, 7, 18, -1);
			mesh.AddFace(20, 8, 10, -1);
			mesh.AddFace(20, 10, 22, -1);
			mesh.AddFace(21, 17, 13, -1);
			mesh.AddFace(21, 13, 9, -1);
			mesh.AddFace(11, 15, 19, -1);
			mesh.AddFace(11, 19, 23, -1);

			mesh.SendToGPUMemory();

			cameraObj.Position = new asd.Vector3DF(0, 0, 10);
			cameraObj.Focus = new asd.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 20.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 20.0f;
			cameraObj.WindowSize = new asd.Vector2DI(800, 600);

			meshObj.AddMesh(mesh);
			meshObj.Rotation = new asd.Vector3DF(20.0f, 20.0f, 0.0f);

			lightObj.Rotation = new asd.Vector3DF(30, 160, 0);
		}
	}
}
