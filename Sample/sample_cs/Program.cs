using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace sample_cs
{
	class Program
	{
		static void Main(string[] args)
		{
			ISample[] samples = 
			{
				new Window.EmptyExternal(),
				new Log(),
				new Graphics.PostEffect.CustomPostEffect(),
			};

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}
