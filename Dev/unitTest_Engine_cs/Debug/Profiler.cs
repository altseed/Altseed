using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ace;

namespace unitTest_Engine_cs.Debug
{
	class MyLayer : Layer2D
	{
	}

	class Profiler : EngineTest
	{
		public Profiler()
			: base(60)
		{
		}

		protected override void OnStart()
		{
			Console.WriteLine("Writeで負荷を掛けます");
		}

		protected override void OnUpdating()
		{
			if(Time == 20)
			{
				Engine.CurrentScene.Layers.First().Name = "Test";
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
