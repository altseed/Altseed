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
			var scene = new ace.Scene();
			var layer = new ace.Layer3D();
			var meshObj = new ace.ModelObject3D();
			var lightObj = new ace.DirectionalLightObject3D();
			var cameraObj = new ace.CameraObject3D();

			scene.AddLayer(layer);
			layer.AddObject(meshObj);
			layer.AddObject(lightObj);
			layer.AddObject(cameraObj);
			ace.Engine.ChangeScene(scene);

			var mesh = ace.Engine.Graphics.CreateMesh();
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, 0.5f), new ace.Vector3DF(0, 0, 1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, 0.5f), new ace.Vector3DF(0, 0, 1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, 0.5f), new ace.Vector3DF(0, 0, 1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, 0.5f), new ace.Vector3DF(0, 0, 1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, -0.5f), new ace.Vector3DF(1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, -0.5f), new ace.Vector3DF(1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, -0.5f), new ace.Vector3DF(0, 0, -1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, -0.5f), new ace.Vector3DF(0, 0, -1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, 0.5f), new ace.Vector3DF(-1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, 0.5f), new ace.Vector3DF(0, 1, 0), new ace.Vector3DF(0, 0, -1), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, 0.5f), new ace.Vector3DF(-1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, 0.5f), new ace.Vector3DF(0, -1, 0), new ace.Vector3DF(0, 0, 1), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, 0.5f), new ace.Vector3DF(1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, 0.5f), new ace.Vector3DF(0, 1, 0), new ace.Vector3DF(0, 0, -1), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, 0.5f), new ace.Vector3DF(1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, 0.5f), new ace.Vector3DF(0, -1, 0), new ace.Vector3DF(0, 0, 1), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, -0.5f), new ace.Vector3DF(0, 0, -1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, 0.5f, -0.5f), new ace.Vector3DF(0, 1, 0), new ace.Vector3DF(0, 0, -1), new ace.Vector2DF(1, 0), new ace.Vector2DF(1, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, -0.5f), new ace.Vector3DF(0, 0, -1), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(0.5f, -0.5f, -0.5f), new ace.Vector3DF(0, -1, 0), new ace.Vector3DF(0, 0, 1), new ace.Vector2DF(1, 1), new ace.Vector2DF(1, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, -0.5f), new ace.Vector3DF(-1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, 0.5f, -0.5f), new ace.Vector3DF(0, 1, 0), new ace.Vector3DF(0, 0, -1), new ace.Vector2DF(0, 0), new ace.Vector2DF(0, 0), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, -0.5f), new ace.Vector3DF(-1, 0, 0), new ace.Vector3DF(0, 1, 0), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);
			mesh.AddVertex(new ace.Vector3DF(-0.5f, -0.5f, -0.5f), new ace.Vector3DF(0, -1, 0), new ace.Vector3DF(0, 0, 1), new ace.Vector2DF(0, 1), new ace.Vector2DF(0, 1), new ace.Color(255, 255, 255, 255), 0, 0);

			mesh.AddFace(0, 2, 3);
			mesh.AddFace(0, 3, 1);
			mesh.AddFace(12, 4, 5);
			mesh.AddFace(12, 5, 14);
			mesh.AddFace(16, 6, 7);
			mesh.AddFace(16, 7, 18);
			mesh.AddFace(20, 8, 10);
			mesh.AddFace(20, 10, 22);
			mesh.AddFace(21, 17, 13);
			mesh.AddFace(21, 13, 9);
			mesh.AddFace(11, 15, 19);
			mesh.AddFace(11, 19, 23);

			mesh.SendToGPUMemory();

			cameraObj.Position = new ace.Vector3DF(0, 0, 10);
			cameraObj.Focus = new ace.Vector3DF(0, 0, 0);
			cameraObj.FieldOfView = 20.0f;
			cameraObj.ZNear = 1.0f;
			cameraObj.ZFar = 20.0f;
			cameraObj.WindowSize = new ace.Vector2DI(800, 600);

			meshObj.AddMesh(mesh);
			meshObj.Rotation = new ace.Vector3DF(20.0f, 20.0f, 0.0f);

			lightObj.Rotation = new ace.Vector3DF(30, 160, 0);
		}
	}
}
