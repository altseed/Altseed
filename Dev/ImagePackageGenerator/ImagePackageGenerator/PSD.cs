using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImagePackageGenerator
{
	enum LayerObjectType
	{
		Image,
		Button
	}

	enum LayerAdditionalObjectType
	{
		None,
		Normal,
		Pressed,
		Hovered
	}

	struct Rect
	{
		public int Top;
		public int Bottom;
		public int Left;
		public int Right;

		public int Width { get { return Right - Left; } }
		public int Height { get { return Bottom - Top; } }

		public override string ToString()
		{
			return string.Format("X = {0}, Y = {1}, Width = {2}, Height = {3}", Left, Top, Width, Height);
		}
	}

	[System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
	struct Color
	{
		public byte R;
		public byte G;
		public byte B;
		public byte A;
	}

	enum ErrorCode
	{
		Succeed,
		FileIsNotFound,
		DepthIs16,
		FailedToParse,
	}

	class PSD
	{
		swig.Document doc;

		List<Layer> layers = new List<Layer>();

		public ICollection<Layer> Layers { get { return layers; } }

		public unsafe ErrorCode Load(string path)
		{
			Reset();

			if (!System.IO.File.Exists(path)) return ErrorCode.FileIsNotFound;

			var buf = System.IO.File.ReadAllBytes(path);

			fixed(byte* p = buf)
			{
				doc = swig.Document.Create((IntPtr)p, buf.Count());
			}

			if (doc == null) return ErrorCode.FailedToParse;

			var depth = doc.GetDepth();
			if(depth > 8)
			{
				doc.Dispose();
				doc = null;
				return ErrorCode.DepthIs16;
			}

			for (int i = 0; i < doc.GetLayerCount(); i++ )
			{
				layers.Add(new Layer(doc.GetLayer(i)));
			}

			return ErrorCode.Succeed;
		}

		public void Terminate()
		{
			foreach (var layer in layers)
			{
				layer.Dispose();
			}

			layers.Clear();

			if(doc != null)
			{
				doc.Dispose();
			}
			doc = null;
		}

		public byte[] Export()
		{
			int version = 1;

			List<byte[]> buffer = new List<byte[]>();

			// 識別子
			buffer.Add(new byte[] { (byte)'a', (byte)'i', (byte)'p', 0 });

			// バージョン
			buffer.Add(BitConverter.GetBytes(version));

			// レイヤーの個数
			buffer.Add(BitConverter.GetBytes(layers.Count));

			// 背景から出力
			foreach (var layer in Layers.Reverse())
			{
				// 名前
				var buf = Encoding.Unicode.GetBytes(layer.Name);
				buffer.Add(BitConverter.GetBytes(buf.Count() / 2 + 1));
				buffer.Add(buf);
				buffer.Add(new byte[] { 0, 0 });

				// 配置領域
				buffer.Add(BitConverter.GetBytes(layer.Rect.Left));
				buffer.Add(BitConverter.GetBytes(layer.Rect.Top));
				buffer.Add(BitConverter.GetBytes(layer.Rect.Width));
				buffer.Add(BitConverter.GetBytes(layer.Rect.Height));

				// ソース領域(Version1より追加)
				buffer.Add(BitConverter.GetBytes(0));
				buffer.Add(BitConverter.GetBytes(0));
				buffer.Add(BitConverter.GetBytes(layer.Rect.Width));
				buffer.Add(BitConverter.GetBytes(layer.Rect.Height));

				// 属性(Version1より追加)
				buffer.Add(BitConverter.GetBytes((int)layer.ObjectType));
				buffer.Add(BitConverter.GetBytes((int)layer.AdditionalObjectType));

				// 画素
				foreach (var c in layer.Pixels)
				{
					buffer.Add(new byte[] { c.R, c.G, c.B, c.A });
				}
			}

			return buffer.SelectMany(_ => _).ToArray();
		}

		void Reset()
		{
			foreach(var layer in layers)
			{
				layer.Dispose();
			}

			layers.Clear();

			if (doc != null)
			{
				doc.Dispose();
			}
			doc = null;

			// 画像を完全に破棄する。
			System.GC.Collect();
			System.GC.Collect();
			System.GC.WaitForPendingFinalizers();
			System.GC.Collect();
			System.GC.Collect();
		}

		public class Layer : IDisposable
		{
			swig.Layer layer;

			public unsafe Layer(swig.Layer layer)
			{
				this.layer = layer;

				Name = layer.GetName();

				var rect = layer.GetRect();
				var Rect_ = new ImagePackageGenerator.Rect();
				Rect_.Top = rect.Top;
				Rect_.Bottom = rect.Bottom;
				Rect_.Left = rect.Left;
				Rect_.Right = rect.Right;
				Rect = Rect_;

				ObjectType = (LayerObjectType)layer.ObjectType;

				AdditionalObjectType = (LayerAdditionalObjectType)layer.AdditionalObjectType;

				Pixels = new Color[Rect.Width * Rect.Height];

				fixed(Color* p_ = Pixels)
				{
					byte* dst = (byte*)p_;
					byte* src = (byte*)layer.GetData();
			
					for(int y = 0; y < Rect.Height; y++)
					{
						for (int x = 0; x < Rect.Width; x++)
						{
							dst[(x + y * Rect.Width) * 4 + 0] = src[x * 4 + y * Rect.Width * 4 + 0];
							dst[(x + y * Rect.Width) * 4 + 1] = src[x * 4 + y * Rect.Width * 4 + 1];
							dst[(x + y * Rect.Width) * 4 + 2] = src[x * 4 + y * Rect.Width * 4 + 2];
							dst[(x + y * Rect.Width) * 4 + 3] = src[x * 4 + y * Rect.Width * 4 + 3];
						}
					}
				}
			}

			public void Dispose()
			{
				Pixels = null;
			}

			public string Name { get; private set; }

			public Rect Rect { get; private set; }

			public LayerObjectType ObjectType { get; private set; }

			public LayerAdditionalObjectType AdditionalObjectType { get; private set; }

			public Color[] Pixels { get; private set; }
		}
	}
}
