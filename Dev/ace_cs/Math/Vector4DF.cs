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
		/// X成分
		/// </summary>
		public float X;

		/// <summary>
		/// Y成分
		/// </summary>
		public float Y;

		/// <summary>
		/// Z成分
		/// </summary>
		public float Z;

		/// <summary>
		/// W成分
		/// </summary>
		public float W;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="x">X成分</param>
		/// <param name="y">Y成分</param>
		/// <param name="z">Z成分</param>
		/// <param name="w">W成分</param>
		public Vector4DF(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		public override string ToString()
		{
			return string.Format("({0}, {1}, {2}, {3})", X, Y, Z, W);
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
				W /= (value / len);
			}
		}
		/// <summary>
		/// ベクトルの長さの二乗を取得する。
		/// </summary>
		public float SquaredLength
		{
			get { return X * X + Y * Y + Z * Z + W * W; }
		}

		/// <summary>
		/// このベクトルの単位ベクトルを取得する。
		/// </summary>
		public Vector4DF Normal
		{
			get
			{
				float length = Length;
				return new Vector4DF(X / length, Y / length, Z / length, W / length);
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
			W /= length;
		}

		public static bool operator ==(Vector4DF left, Vector4DF right)
		{
			return left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W;
		}
		public static bool operator !=(Vector4DF left, Vector4DF right)
		{
			return left.X != right.X || left.Y != right.Y || left.Z != right.Z || left.W != right.W;
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

		/// <summary>
		/// 外積を取得する。
		/// </summary>
		/// <param name="v1">v1ベクトル</param>
		/// <param name="v2">v2ベクトル</param>
		/// <returns>外積v1×v2</returns>
		public static float Dot(Vector4DF v1, Vector4DF v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W;
		}

		/// <summary>
		/// 2点間の距離を取得する。
		/// </summary>
		/// <param name="v1">v1ベクトル</param>
		/// <param name="v2">v2ベクトル</param>
		/// <returns>v1とv2の距離</returns>
		public static float Distance(Vector4DF v1, Vector4DF v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			float dz = v1.Z - v2.Z;
			float dw = v1.W - v2.W;
			return (float)Math.Sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
		}
	}
}
