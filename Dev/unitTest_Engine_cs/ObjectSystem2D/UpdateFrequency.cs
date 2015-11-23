using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace unitTest_Engine_cs.ObjectSystem2D
{
	class UpdateFrequency : EngineTest
	{
		class MovingObject : TextureObject2D
		{
			protected override void OnUpdate()
			{
				Position += new Vector2DF(3, 0);
			}
		}

		public UpdateFrequency() : base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			Engine.ChangeScene(scene);

			AddHastedLayerTo(scene, 2, new Vector2DF(10, 10));
			AddHastedLayerTo(scene, 1, new Vector2DF(10, 110));
			AddHastedLayerTo(scene, 0.2f, new Vector2DF(10, 210));
			AddHastedLayerTo(scene, 0, new Vector2DF(10, 310));
		}

		private void AddHastedLayerTo(Scene scene, float updateFrequency, Vector2DF position)
		{
			var layer = new Layer2D()
			{
				UpdateFrequency = updateFrequency
			};
			var obj = new MovingObject()
			{
				Texture = Engine.Graphics.CreateTexture2D(CloudTexturePath),
				Scale = new Vector2DF(0.5f, 0.5f),
				Position = position,
			};
			scene.AddLayer(layer);
			layer.AddObject(obj);
		}
	}
}
