using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	static class Extensions
	{
		public static bool InputInt(this asd.Tool tool, string label, ref int value)
		{
			var buf = new int[1] { value };
			var changed = tool.InputInt(label, buf);
			value = buf[0];
			return changed;
		}

		public static bool Color(this asd.Tool tool, string label, ref asd.Color color)
		{
			var buf = new float[4] 
			{
				color.R / 255.0f,
				color.G / 255.0f,
				color.B / 255.0f,
				color.A / 255.0f
			};
			var changed = tool.ColorEdit4(label, buf);
			color = new asd.Color((byte)(buf[0] * 255),
				(byte)(buf[1] * 255),
				(byte)(buf[2] * 255),
				(byte)(buf[3] * 255));
			return changed;
		}
	}
}
