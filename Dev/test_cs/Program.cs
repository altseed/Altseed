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
				new Window.Empty(),
				new Window.EmptyExternal(),
				new LogSample(),
                new KeyboardSample(),
                new MouseSample1(),
                new MouseSample2(),
                new JoystickSample(),
				new Graphics.PostEffect.CustomPostEffect(),
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
			var initialized = ace.Engine.Initialize("test", 640, 480, false);

			while (ace.Engine.DoEvents())
			{
				ace.Engine.Update();
			}

			ace.Engine.Terminate();
		}
	}
}
