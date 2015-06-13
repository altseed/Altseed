using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public static class MathHelper
	{
		const float ReversePI = 1 / (float)Math.PI;
		const float Reverse180 = 1 / 180.0f;

		public static float RadianToDegree(float radian)
		{
			return radian * ReversePI * 180;
		}

		public static float DegreeToRadian(float degree)
		{
			return degree * Reverse180 * (float)Math.PI;
		}

		public static T Clamp<T>(T v, T max, T min) where T : System.IComparable<T>
		{
			if (v.CompareTo(max) > 0)
			{
				v = max;
			}

			if (v.CompareTo(min) < 0)
			{
				v = min;
			}

			return v;
		}
	}
}
