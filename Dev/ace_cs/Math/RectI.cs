using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public struct RectI
	{
		public int X { get; set; }
		public int Y { get; set; }
		public int Width { get; set; }
		public int Height { get; set; }

		public Vector2DI Position
		{
			get { return new Vector2DI( X, Y ); }
		}

		public Vector2DI Size
		{
			get { return new Vector2DI( Width, Height ); }
		}

		public RectI(int x, int y, int width, int height)
			: this()
		{
			this.X = x;
			this.Y = y;
			this.Width = width;
			this.Height = height;
		}
	}
}
