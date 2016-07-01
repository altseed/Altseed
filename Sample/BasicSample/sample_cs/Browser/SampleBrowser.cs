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
        public static readonly float Margin = 16;

		private ISample[] samples;

		public ISample Selected = null;

		public SampleBrowser(ISample[] samples)
		{
			this.samples = samples;
		}

		public void Run()
		{
			while(true)
			{
				Engine.Initialize("サンプルブラウザ", 640, 480, new EngineOption());

				var scene = new Scene();
				var layer = new SampleBrowserLayer(this, samples);

                var viewSize = SampleBrowserLayer.Columns * SampleBrowserLayer.ItemOffset.Y;
                var size = (480 - 80) * viewSize / layer.TotalHeight;
                var infoLayer = new SampleInfoLayer(size, layer.TotalHeight, viewSize) { DrawingPriority = 2 };
                layer.SelectionChanged += s => infoLayer.Show(s);

				Engine.ChangeScene(scene);
				scene.AddLayer(layer);
                scene.AddLayer(infoLayer);

				var hintLayer = new Layer2D();
				hintLayer.AddObject(new TextureObject2D()
				{
					Texture = Engine.Graphics.CreateTexture2D("Data/Browser/Hint.png")
				});

				scene.AddLayer(hintLayer);

				while(Engine.DoEvents() && Selected == null)
				{
					Engine.Update();
                    infoLayer.MoveScrollBar(layer.CameraArea.Y);
				}

				Engine.Terminate();

				if(Selected == null)
				{
					break;
				}
				Selected.Run();
				Selected = null;
			}
		}
	}
}
