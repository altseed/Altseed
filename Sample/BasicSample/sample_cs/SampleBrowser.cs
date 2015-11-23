using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd;

namespace sample_cs
{
	class SampleBrowser
	{
		private ISample[] samples;

		public SampleBrowser(ISample[] samples)
		{
			this.samples = samples;
		}

		public void Run()
		{
			while(true)
			{
				ISample selected = null;

				Engine.Initialize("サンプルブラウザ", 640, 480, new EngineOption());

				var scene = new Scene();
				var layer = new SampleBrowserLayer(samples);
				layer.OnDecide += s => selected = s;

				Engine.ChangeScene(scene);
				scene.AddLayer(layer);

				var hintLayer = new Layer2D();
				hintLayer.AddObject(new TextureObject2D()
				{
					Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Hint.png")
				});

				scene.AddLayer(hintLayer);

				while(Engine.DoEvents() && selected == null)
				{
					Engine.Update();
				}

				Engine.Terminate();

				if(selected == null)
				{
					break;
				}
				selected.Run();
			}
		}
	}
}
