using asd;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs.Input
{
	class BundleController : EngineTest
	{
		enum KeyState
		{

		}

		TextObject2D textObject;

		public BundleController(int timeForExit) : base(300)
		{
		}

		protected override void OnStart()
		{
			textObject = new TextObject2D()
			{
				Font = Engine.Graphics.CreateDynamicFont("", 24, new Color(255, 255, 255), 0, new Color()),
				Position = new Vector2DF(10, 10),
			};
			Engine.AddObject2D(textObject);
		}

		protected override void OnUpdating()
		{
		}
	}
}
