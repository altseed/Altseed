using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 4次元ベクトル
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct Vector4DF
	{
		private static Vector3DF zero = new Vector3DF();

		/// <summary>
		/// X
		/// </summary>
		public float X;

		/// <summary>
		/// Y
		/// </summary>
		public float Y;

		/// <summary>
		/// Z
		/// </summary>
		public float Z;

		/// <summary>
		/// W
		/// </summary>
		public float W;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="x">X</param>
		/// <param name="y">Y</param>
		/// <param name="z">Z</param>
		/// <param name="w">W</param>
		public Vector4DF(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		public override string ToString()
		{
			return string.Format("({0}, {1}, {2})", X, Y, Z);
		}

		public static bool operator ==(Vector4DF left, Vector4DF right)
		{
			return left.X == right.X && left.Y == right.Y;
		}
		public static bool operator !=(Vector4DF left, Vector4DF right)
		{
			return left.X != right.X || left.Y != right.Y;
		}
		public static Vector4DF operator +(Vector4DF left, Vector4DF right)
		{
			return new Vector4DF(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
		}
		public static Vector4DF operator -(Vector4DF left, Vector4DF right)
		{
			return new Vector4DF(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
		}
		public static Vector4DF operator -(Vector4DF op)
		{
			return new Vector4DF(-op.X, -op.Y, -op.Z, -op.W);
		}
		public static Vector4DF operator *(Vector4DF op, float scolar)
		{
			return new Vector4DF(op.X * scolar, op.Y * scolar, op.Z * scolar, op.W * scolar);
		}
		public static Vector4DF operator *(float scolar, Vector4DF op)
		{
			return new Vector4DF(scolar * op.X, scolar * op.Y, scolar * op.Z, scolar * op.W);
		}
		public static Vector4DF operator *(Vector4DF left, Vector4DF right)
		{
			return new Vector4DF(left.X * right.X, left.Y * right.Y, left.Z * right.Z, left.W * right.W);
		}
		public static Vector4DF operator /(Vector4DF op, float scolar)
		{
			return new Vector4DF(op.X / scolar, op.Y / scolar, op.Z / scolar, op.W / scolar);
		}
		public static Vector4DF operator /(Vector4DF left, Vector4DF right)
		{
			return new Vector4DF(left.X / right.X, left.Y / right.Y, left.Z / right.Z, left.W / right.W);
		}
	}
}
