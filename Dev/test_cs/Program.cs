using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace test_cs
{
	class Program
	{
		static void Main(string[] args)
		{
			ISample[] samples = 
			{
				new Empty(),
				new LogSample(),
				new ObjectSystem.ObjectSimple(),
				new ObjectSystem.ParentObject(),
				new ObjectSystem.CustomObject(),
				new ObjectSystem.Component(),
			};

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}

	class Empty : ISample
	{
		[STAThread]
		public void Run()
		{
			var option = new ace.EngineOption
			{
				GraphicsType = ace.GraphicsType.DirectX11,
				IsFullScreen = false
			};
			var initialized = ace.Engine.Initialize("test", 640, 480, option);

			while (ace.Engine.DoEvents())
			{
				ace.Engine.Update();
			}

			ace.Engine.Terminate();
		}
	}
}
