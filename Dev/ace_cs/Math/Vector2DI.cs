using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct Vector2DI
	{
		private static Vector2DI zero = new Vector2DI();

		public int X;
		public int Y;
	}
}
