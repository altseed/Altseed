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
		/// 指定した X 成分と Y 成分を使用して、ace.Vector2DF の新しいインスタンスを初期化する。
		/// </summary>
		/// <param name="x">X成分。</param>
		/// <param name="y">Y成分。</param>
		public Vector2DF( float x, float y )
			: this()
		{
			this.X = x;
			this.Y = y;
		}

		public float X;
		public float Y;

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
	}
}
