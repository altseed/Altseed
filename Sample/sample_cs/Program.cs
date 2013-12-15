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
				new Window.Empty(),
				new Window.EmptyExternal(),

				new LogSample(),

                new Input.KeyboardSample(),
                new Input.MouseSample1(),
                new Input.MouseSample2(),
                new Input.JoystickSample(),

				new Graphics.PostEffect.CustomPostEffect(),
			};

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}
