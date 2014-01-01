using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	[System.Runtime.InteropServices.StructLayout( System.Runtime.InteropServices.LayoutKind.Sequential )]
	public struct Vector2DF
	{
		private static Vector2DF zero = new Vector2DF();

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
		/// ベクトルの X 要素。
		/// </summary>
		public float X;
		/// <summary>
		/// ベクトルの Y 要素。
		/// </summary>
		public float Y;

		/// <summary>
		/// ベクトルの長さを取得または設定します。
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
		/// ベクトルの長さの二乗を取得または設定します。
		/// </summary>
		public float SquaredLength
		{
			get { return X * X + Y * Y; }
		}
		/// <summary>
		/// ベクトルの向きを弧度法で取得または設定します。
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
		/// ベクトルの向きを度数法で取得または設定します。
		/// </summary>
		public float Degree
		{
			get { return MathHelper.RadianToDegree( Radian ); }
			set { Radian = MathHelper.DegreeToRadian( value ); }
		}

		public override string ToString()
		{
			return string.Format( "({0}, {1})", X, Y );
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
	}
}
