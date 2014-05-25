using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	public struct Color
	{
		public Color(byte r, byte g, byte b, byte a) : this()
		{
			this.Red = r;
			this.Green = g;
			this.Blue = b;
			this.Alpha = a;
		}

		public byte Red { get; set; }
		public byte Green { get; set; }
		public byte Blue { get; set; }
		public byte Alpha { get; set; }
	}
}
