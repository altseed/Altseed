using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 3次元ベクトル
	/// </summary>
	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	public struct Vector3DF
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
		/// コンストラクタ
		/// </summary>
		/// <param name="x">X成分</param>
		/// <param name="y">Y成分</param>
		/// <param name="z">Z成分</param>
		public Vector3DF(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		public override string ToString()
		{
			return string.Format("({0}, {1}, {2})", X, Y, Z);
		}

		/// <summary>
		/// ベクトルの長さを取得または設定する。
		/// </summary>
		public float Length
		{
			get { return (float)Math.Sqrt(SquaredLength); }
			set
			{
				var len = Length;
				X /= (value / len);
				Y /= (value / len);
				Z /= (value / len);

			}
		}
		/// <summary>
		/// ベクトルの長さの二乗を取得する。
		/// </summary>
		public float SquaredLength
		{
			get { return X * X + Y * Y + Z * Z; }
		}

		/// <summary>
		/// このベクトルの単位ベクトルを取得する。
		/// </summary>
		public Vector3DF Normal
		{
			get
			{
				float length = Length;
				return new Vector3DF(X / length, Y / length, Z / length);
			}
		}

		/// <summary>
		/// このベクトルを単位ベクトル化する。
		/// </summary>
		public void Normalize()
		{
			float length = Length;
			X /= length;
			Y /= length;
			Z /= length;
		}

		public static bool operator ==(Vector3DF left, Vector3DF right)
		{
			return left.X == right.X && left.Y == right.Y && left.Z == right.Z;
		}
		public static bool operator !=(Vector3DF left, Vector3DF right)
		{
			return left.X != right.X || left.Y != right.Y || left.Z != right.Z;
		}
		public static Vector3DF operator +(Vector3DF left, Vector3DF right)
		{
			return new Vector3DF(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
		}
		public static Vector3DF operator -(Vector3DF left, Vector3DF right)
		{
			return new Vector3DF(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
		}
		public static Vector3DF operator -(Vector3DF op)
		{
			return new Vector3DF(-op.X, -op.Y, -op.Z);
		}
		public static Vector3DF operator *(Vector3DF op, float scolar)
		{
			return new Vector3DF(op.X * scolar, op.Y * scolar, op.Z * scolar);
		}
		public static Vector3DF operator *(float scolar, Vector3DF op)
		{
			return new Vector3DF(scolar * op.X, scolar * op.Y, scolar * op.Z);
		}
		public static Vector3DF operator *(Vector3DF left, Vector3DF right)
		{
			return new Vector3DF(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
		}
		public static Vector3DF operator /(Vector3DF op, float scolar)
		{
			return new Vector3DF(op.X / scolar, op.Y / scolar, op.Z / scolar);
		}
		public static Vector3DF operator /(Vector3DF left, Vector3DF right)
		{
			return new Vector3DF(left.X / right.X, left.Y / right.Y, left.Z / right.Z);
		}

		/// <summary>
		/// 内積を取得する。
		/// </summary>
		/// <param name="v1">v1ベクトル</param>
		/// <param name="v2">v2ベクトル</param>
		/// <returns>内積v1・v2</returns>
		public static float Dot(Vector3DF v1, Vector3DF v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
		}

		/// <summary>
		/// 外積を取得する。
		/// </summary>
		/// <param name="v1">v1ベクトル</param>
		/// <param name="v2">v2ベクトル</param>
		/// <returns>外積v1×v2</returns>
		public static Vector3DF Cross( Vector3DF v1, Vector3DF v2 )
		{
			Vector3DF o;
			float x = v1.Y * v2.Z - v1.Z * v2.Y;
			float y = v1.Z * v2.X - v1.X * v2.Z;
			float z = v1.X * v2.Y - v1.Y * v2.X;
			o.X = x;
			o.Y = y;
			o.Z = z;
			return o;
		}

		/// <summary>
		/// 2点間の距離を取得する。
		/// </summary>
		/// <param name="v1">v1ベクトル</param>
		/// <param name="v2">v2ベクトル</param>
		/// <returns>v1とv2の距離</returns>
		public static float Distance(Vector3DF v1, Vector3DF v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			float dz = v1.Z - v2.Z;
			return (float)Math.Sqrt(dx * dx + dy * dy + dz * dz);
		}
	}
}
