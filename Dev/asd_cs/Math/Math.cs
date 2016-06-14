using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public static class MathHelper
	{
		public static float RadianToDegree(float radian)
		{
			const float ReversePI = 1 / (float)Math.PI;

			return radian * ReversePI * 180;
		}

		public static float DegreeToRadian(float degree)
		{
			const float Reverse180 = 1 / 180.0f;

			return degree * Reverse180 * (float)Math.PI;
		}

		public static T Clamp<T>(T v, T max, T min) where T : System.IComparable<T>
		{
			if (asd.Particular.Helper.CompareTo(v, max) > 0)
			{
				v = max;
			}

			if (asd.Particular.Helper.CompareTo(v, min) < 0)
			{
				v = min;
			}

			return v;
		}
	}
}
