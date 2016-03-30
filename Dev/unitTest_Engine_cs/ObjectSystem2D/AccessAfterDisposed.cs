using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class AccessAfterDisposed : EngineTest
	{
		public AccessAfterDisposed() : base(30)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer = new Layer2D();
			var textureObject = new TextureObject2D
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath)
			};
			var textObject = new TextObject2D
			{
				Font = Engine.Graphics.CreateFont("Data/Font/meiryoTest.aff")
			};
			var geometoryObject = new GeometryObject2D
			{
				Shape = new RectangleShape()
				{
					DrawingArea = new RectF(0, 0, 100, 100)
				}
			};

			Engine.ChangeScene(scene);
			scene.AddLayer(layer);
			layer.AddObject(textureObject);
			layer.AddObject(textObject);
			layer.AddObject(geometoryObject);

			scene.Dispose();

			Assert.AreEqual(false, scene.IsAlive);
			Assert.AreEqual(false, layer.IsAlive);
			Assert.AreEqual(false, textureObject.IsAlive);
			Assert.AreEqual(false, textObject.IsAlive);
			Assert.AreEqual(false, geometoryObject.IsAlive);

			var tex = textureObject.Texture;
			var font = textObject.Font;
			var col = geometoryObject.Color;
		}
	}
}
