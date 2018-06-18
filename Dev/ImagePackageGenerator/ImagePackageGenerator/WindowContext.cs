using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImagePackageGenerator
{
	class WindowContext
	{
		PSD psd = new PSD();

		public List<Layer> Layers { get; private set; }

		public WindowContext()
		{
			Layers = new List<Layer>();
		}

		public void Terminate()
		{
			if (psd != null)
			{
				psd.Terminate();
			}
			psd = null;
		}

		public byte[] Export()
		{
			return psd.Export();
		}

		public void Load(string path)
		{
			foreach(var layer in Layers)
			{
				layer.Dispose();
			}
			Layers.Clear();

			var ec = psd.Load(path);

			if (ec == ErrorCode.DepthIs16)
			{
				System.Windows.Forms.MessageBox.Show("8bit画像のみサポートしています。\npsdファイルのチャンネルを確認するようお願いします。");
				return;
			}
			else if (ec != ErrorCode.Succeed)
			{
				System.Windows.Forms.MessageBox.Show("読込に失敗しました。");
				return;
			}

			foreach (var l in psd.Layers)
			{
				Layers.Add(new Layer(l));
			}
		}

		public class Layer : IDisposable
		{ 
			PSD.Layer layer;

			public unsafe Layer(PSD.Layer layer)
			{
				this.layer = layer;

				Name = layer.Name;
				Rect = layer.Rect;

				var wi = new System.Drawing.Bitmap(Rect.Width, Rect.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

				fixed (Color* p = layer.Pixels)
				{
					var acc = wi.LockBits(new System.Drawing.Rectangle(0, 0, wi.Width, wi.Height),
						System.Drawing.Imaging.ImageLockMode.ReadWrite,
						System.Drawing.Imaging.PixelFormat.Format32bppArgb);

					for (int y = 0; y < Rect.Height; y++)
					{
						for (int x = 0; x < Rect.Width; x++)
						{
							var src = *(p + x + y * Rect.Width);
							var dst = (Color*)acc.Scan0 + x + y * Rect.Width;

							var r = src.R;
							var b = src.B;
							src.B = r;
							src.R = b;

							*dst = src;
						}
					}

					wi.UnlockBits(acc);
				}

				Image = wi;
			}

			public string Name { get; private set; }

			public Rect Rect { get; private set; }

			public System.Drawing.Bitmap Image { get; private set; }

			public void Dispose()
			{
				if(Image != null)
				{
					Image.Dispose();
				}
				Image = null;
			}

			public override string ToString()
			{
				return Name;
			}
		}
	}
}
