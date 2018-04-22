using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Tool
{
	class ShowButton : EngineTest
	{
		private asd.TextureObject2D obj;

		public ShowButton() : base(120)
		{
		}

		protected override void OnStart()
		{
			obj = new asd.TextureObject2D()
			{
				Texture = asd.Engine.Graphics.CreateTexture2D("Data/Texture/Sample1.png"),
				Position = new asd.Vector2DF(10, 10),
				IsDrawn = false,
			};
			asd.Engine.AddObject2D(obj);

			asd.Engine.OpenTool();
		}

		protected override void OnUpdating()
		{
			asd.Engine.Tool.Begin("ShowButton");

			if (asd.Engine.Tool.Button("Push me!"))
			{
				obj.IsDrawn = true;
			}

			asd.Engine.Tool.End();
		}

		protected override void OnFinish()
		{
			asd.Engine.CloseTool();
		}
	}
}
