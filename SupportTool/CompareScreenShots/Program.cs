using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CompareScreenShots
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Count() < 3) return;

			List<string> results = new List<string>();

			var originImages = System.IO.Directory.GetFiles(args[0], "*.png", System.IO.SearchOption.TopDirectoryOnly);

			foreach (var originImage in originImages)
			{
				var img = new System.Drawing.Bitmap(originImage);

				var dx = args[1] + System.IO.Path.GetFileNameWithoutExtension(originImage) + "_dx.png";
				var gl = args[1] + System.IO.Path.GetFileNameWithoutExtension(originImage) + "_gl.png";

				results.Add("-- " + System.IO.Path.GetFileNameWithoutExtension(originImage));

				if (System.IO.File.Exists(dx))
				{
					var target = new System.Drawing.Bitmap(dx);
					var sum = GetDiff(img, target);
					results.Add( sum + " " + dx);
				}
				else
				{
					results.Add("NotFound " + dx);
				}

				if (System.IO.File.Exists(gl))
				{
					var target = new System.Drawing.Bitmap(gl);
					var sum = GetDiff(img, target);
					results.Add(sum + " " + gl);
				}
				else
				{
					results.Add("NotFound " + gl);
				}

				results.Add("");
			}

			System.IO.File.WriteAllLines(args[2], results);
		}

		static int GetDiff(System.Drawing.Bitmap origin, System.Drawing.Bitmap target)
		{
			int sum = 0;
			for (int y = 0; y < Math.Max(origin.Height, target.Height); y++)
			{
				for (int x = 0; x < Math.Max(origin.Width, target.Width); x++)
				{
					System.Drawing.Color co = new System.Drawing.Color();
					System.Drawing.Color ct = new System.Drawing.Color();

					if ( origin.Width > x &&
						origin.Height > y)
					{
						co = origin.GetPixel(x, y);
					}

					if (target.Width > x &&
						target.Height > y)
					{
						ct = target.GetPixel(x, y);
					}

					sum += (Math.Abs(co.R - ct.R) + Math.Abs(co.G - ct.G) + Math.Abs(co.B - ct.B) + Math.Abs(co.A - ct.A));
				}
			}

			return sum;
		}
	}
}
