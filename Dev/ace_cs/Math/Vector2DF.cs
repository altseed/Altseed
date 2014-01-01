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

		public float X;
		public float Y;

		public float GetLength()
		{
			return (float)Math.Sqrt( GetSquaredLength() );
		}

		private float GetSquaredLength()
		{
			return X * X + Y * Y;
		}

		public override string ToString()
		{
			return string.Format( "({0}, {1})", X, Y );
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
