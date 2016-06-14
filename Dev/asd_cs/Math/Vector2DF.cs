using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 2次元ベクトル
	/// </summary>
	[System.Runtime.InteropServices.StructLayout( System.Runtime.InteropServices.LayoutKind.Sequential )]
	public struct Vector2DF
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="x">X成分</param>
		/// <param name="y">Y成分</param>
		public Vector2DF( float x, float y )
			: this()
		{
			this.X = x;
			this.Y = y;
		}

		/// <summary>
		/// X成分
		/// </summary>
		public float X;
		/// <summary>
		/// Y成分
		/// </summary>
		public float Y;

		/// <summary>
		/// ベクトルの長さを取得または設定する。
		/// </summary>
		public float Length
		{
			get { return (float)Math.Sqrt( SquaredLength ); }
			set
			{
				float angle = Radian;
				X = (float)Math.Cos( angle ) * value;
				Y = (float)Math.Sin( angle ) * value;
			}
		}
		/// <summary>
		/// ベクトルの長さの二乗を取得する。
		/// </summary>
		public float SquaredLength
		{
			get { return X * X + Y * Y; }
		}

		/// <summary>
		/// このベクトルの単位ベクトルを取得する。
		/// </summary>
		public Vector2DF Normal
		{
			get
			{
				float length = Length;
				return new Vector2DF(X / length, Y / length);
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
		}

		/// <summary>
		/// ベクトルの向きを弧度法で取得または設定する。
		/// </summary>
		public float Radian
		{
			get { return (float)Math.Atan2( Y, X ); }
			set
			{
				float length = Length;
				X = (float)Math.Cos( value ) * length;
				Y = (float)Math.Sin( value ) * length;
			}
		}
		/// <summary>
		/// ベクトルの向きを度数法で取得または設定する。
		/// </summary>
		public float Degree
		{
			get { return MathHelper.RadianToDegree( Radian ); }
			set { Radian = MathHelper.DegreeToRadian( value ); }
		}

		/// <summary>
		/// Vector2DI型に変換する。
		/// </summary>
		/// <returns>Vector2DI型</returns>
		public Vector2DI To2DI()
		{
			return new Vector2DI((int)X, (int)Y);
		}

		public bool Equals(Vector2DF other)
		{
			return X == other.X && Y == other.Y;
		}

		public override string ToString()
		{
			return asd.Particular.Helper.Format( "({0}, {1})", X, Y );
		}

		public static bool operator ==( Vector2DF left, Vector2DF right )
		{
			return left.X == right.X && left.Y == right.Y;
		}
		public static bool operator !=( Vector2DF left, Vector2DF right )
		{
			return left.X != right.X || left.Y != right.Y;
		}
		public static Vector2DF operator +( Vector2DF left, Vector2DF right )
		{
			return new Vector2DF( left.X + right.X, left.Y + right.Y );
		}
		public static Vector2DF operator -( Vector2DF left, Vector2DF right )
		{
			return new Vector2DF( left.X - right.X, left.Y - right.Y );
		}
		public static Vector2DF operator -( Vector2DF op )
		{
			return new Vector2DF( -op.X, -op.Y );
		}
		public static Vector2DF operator *( Vector2DF op, float scolar )
		{
			return new Vector2DF( op.X * scolar, op.Y * scolar );
		}
		public static Vector2DF operator *( float scolar, Vector2DF op )
		{
			return new Vector2DF( scolar * op.X, scolar * op.Y );
		}
		public static Vector2DF operator *( Vector2DF left, Vector2DF right )
		{
			return new Vector2DF( left.X * right.X, left.Y * right.Y );
		}
		public static Vector2DF operator /( Vector2DF op, float scolar )
		{
			return new Vector2DF( op.X / scolar, op.Y / scolar );
		}
		public static Vector2DF operator /( Vector2DF left, Vector2DF right )
		{
			return new Vector2DF( left.X / right.X, left.Y / right.Y );
		}

        /// <summary>
        /// 外積を取得する。
        /// </summary>
        /// <param name="v1">v1ベクトル</param>
        /// <param name="v2">v2ベクトル</param>
        /// <returns>外積v1×v2</returns>
        public static float Cross(Vector2DF v1, Vector2DF v2)
        {
            return v1.X * v2.Y - v1.Y * v2.X;
        }

        /// <summary>
        /// 内積を取得する。
        /// </summary>
        /// <param name="v1">v1ベクトル</param>
        /// <param name="v2">v2ベクトル</param>
        /// <returns>内積v1・v2</returns>
        public static float Dot(Vector2DF v1, Vector2DF v2)
		{
			return v1.X * v2.X + v1.Y * v2.Y;
		}

		/// <summary>
		/// 2点間の距離を取得する。
		/// </summary>
        /// <param name="v1">v1ベクトル</param>
        /// <param name="v2">v2ベクトル</param>
		/// <returns>距離</returns>
		public static float Distance(Vector2DF v1, Vector2DF v2)
		{
			float dx = v1.X - v2.X;
			float dy = v1.Y - v2.Y;
			return (float)Math.Sqrt(dx * dx + dy * dy);
		}
	}
}
