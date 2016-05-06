using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D.LifeCycle
{
	class AccessAfterDisposed : EngineTest
	{
		private Scene scene;
		private Layer2D layer;
		private Layer2D layer2;
		private TextureObject2D textureObject;
		private TextObject2D textObject;
		private GeometryObject2D geometoryObject;

		public AccessAfterDisposed() : base(30)
		{
		}

		protected override void OnStart()
		{
			scene = new Scene();
			layer = new Layer2D();
			layer2 = new Layer2D();
			textureObject = new TextureObject2D
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath)
			};
			textObject = new TextObject2D
			{
				Font = Engine.Graphics.CreateFont("Data/Font/meiryoTest.aff")
			};
			geometoryObject = new GeometryObject2D
			{
				Shape = new RectangleShape()
				{
					DrawingArea = new RectF(0, 0, 100, 100)
				}
			};

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			scene.AddLayer(layer2);
			layer.AddObject(textureObject);
			layer.AddObject(textObject);
			layer.AddObject(geometoryObject);

			var tex = textureObject.Texture;
			var font = textObject.Font;
			var col = geometoryObject.Color;
		}

		protected override void OnUpdating()
		{
			if (Time == 10)
			{
				scene.RemoveLayer(layer2);
				scene.Dispose();
			}
			if (Time == 11)
			{
				Assert.AreEqual(false, scene.IsAlive);
				Assert.AreEqual(false, layer.IsAlive);
				Assert.AreEqual(true, layer2.IsAlive);
				Assert.AreEqual(false, textureObject.IsAlive);
				Assert.AreEqual(false, textObject.IsAlive);
				Assert.AreEqual(false, geometoryObject.IsAlive);
			}
		}
	}
}
