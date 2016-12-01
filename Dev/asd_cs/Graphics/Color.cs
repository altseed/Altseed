using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct Color : IEquatable<Color>
	{
		public byte R;
		public byte G;
		public byte B;
		public byte A;

		public Color(byte r, byte g, byte b, byte a)
		{
			R = r;
			G = g;
			B = b;
			A = a;
		}

		public Color(int r, int g, int b, int a)
		{
			R = Particular.Helper.ConvertColor(r);
			G = Particular.Helper.ConvertColor(g);
			B = Particular.Helper.ConvertColor(b);
			A = Particular.Helper.ConvertColor(a);
		}

		public Color(byte r, byte g, byte b)
			: this(r, g, b, (byte)255)
		{
		}

		public bool Equals(Color other)
		{
			return R == other.R && G == other.G && B == other.B && A == other.A;
		}

		public static bool operator ==(Color lhs, Color rhs)
		{
			return lhs.Equals(rhs);
		}

		public static bool operator !=(Color lhs, Color rhs)
		{
			return !lhs.Equals(rhs);
		}
	}
}
