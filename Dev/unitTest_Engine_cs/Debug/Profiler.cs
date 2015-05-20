using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.Debug
{
	class Profiler : EngineTest
	{
		public Profiler()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			var scene = new Scene();
			var layer1 = new Layer2D() { Name = "Test1" };
			var layer2 = new Layer2D() { Name = "Test2" };

			var texture = Engine.Graphics.CreateTexture2D(CloudTexturePath);
            for(int i = 0; i < 100; i++)
			{
				var obj = new TextureObject2D
				{
					Texture = texture,
				};
				layer1.AddObject(obj);
			}

			Engine.ChangeScene(scene);
			scene.AddLayer(layer1);
			scene.AddLayer(layer2);

			Console.WriteLine("Writeで負荷を掛けます");
		}

		protected override void OnUpdating()
		{
			if(Time == 20)
			{
				Engine.ProfilerIsVisible = true;
			}

			Engine.Profiler.Start(9);
			Console.Write("*");
			Engine.Profiler.End(9);

			Engine.Profiler.Start(12);
		}

		protected override void OnUpdated()
		{
			Engine.Profiler.End(12);
		}
	}
}
