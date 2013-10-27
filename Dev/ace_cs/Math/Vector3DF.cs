using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct Vector3DF
	{
		private static Vector3DF zero = new Vector3DF();

		public float X;
		public float Y;
		public float Z;

		public Vector3DF(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
	}
}
