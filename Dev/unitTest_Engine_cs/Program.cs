using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace unitTest_Engine_cs
{
	class Program
	{
		static void Main(string[] args)
		{
			var e = new Window.Empty();
			e.Test(ace.GraphicsType.DirectX11);
		}
	}

	public abstract class TestFramework
	{
		public abstract void Test(ace.GraphicsType graphicsType);
	}
}
