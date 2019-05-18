using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImagePackageGenerator
{
	class ToolScene : asd.Scene
	{
		PSD psd = null;

		public List<Layer> psdLayers { get; private set; }

		int selectedItem = -1;

		public ToolScene()
		{
			psd = new PSD();
			psdLayers = new List<Layer>();
		}

		protected override void OnRegistered()
		{
			// Causes an error
			//asd.Engine.OpenTool();
		}

		protected override void OnUnregistered()
		{
			Terminate();
		}

		protected override void OnUpdating()
		{

		}
		protected override void OnUpdated()
		{

			if (asd.Engine.Tool.BeginMainMenuBar())
			{


				if (asd.Engine.Tool.BeginMenu("ファイル"))
				{
					var selected = new[] { false };

					if(asd.Engine.Tool.MenuItem("開く", null, selected))
					{
						Open();
					}

					if(asd.Engine.Tool.MenuItem("出力", null, selected))
					{
						Save();
					}

					asd.Engine.Tool.EndMenu();
				}

				asd.Engine.Tool.EndMainMenuBar();
			}

			if (asd.Engine.Tool.BeginFullscreen("ImagePackageGenerator", 30))
			{
				var wsize = asd.Engine.WindowSize;

				asd.Engine.Tool.Columns(2);

				{
					asd.Engine.Tool.SetColumnWidth(0, (int)((wsize.X - 20) * 0.25));

					var ref_selectedItem = new[] { selectedItem };
					var lists = string.Join(";", psdLayers.Select(_ => _.Name.Replace(";", ":")));

					if (asd.Engine.Tool.ListBox("##List", ref_selectedItem, lists))
					{
						selectedItem = ref_selectedItem[0];
					}

					// Into
					if (selectedItem >= 0 && psdLayers.Count > selectedItem)
					{
						asd.Engine.Tool.Text("X = " + psdLayers[selectedItem].Rect.Left);
						asd.Engine.Tool.Text("Y = " + psdLayers[selectedItem].Rect.Right);
						asd.Engine.Tool.Text("width  = " + psdLayers[selectedItem].Rect.Width);
						asd.Engine.Tool.Text("Height = " + psdLayers[selectedItem].Rect.Height);
					}
					else
					{
						asd.Engine.Tool.Text("Not selected.");
					}
				}

				asd.Engine.Tool.NextColumn();

				{
					var max_width = (int)((wsize.X - 20) * 0.75);

					if (selectedItem >= 0 && psdLayers.Count > selectedItem)
					{
						var layer = psdLayers[selectedItem];

						if(max_width > layer.Rect.Width)
						{
							asd.Engine.Tool.Image(layer.Image, new asd.Vector2DF(layer.Rect.Width, layer.Rect.Height));
						}
						else
						{
							var w = max_width;
							var h = (int)(layer.Rect.Height / ((float)layer.Rect.Width / max_width));
							asd.Engine.Tool.Image(layer.Image, new asd.Vector2DF(w,h));
						}
					}
				}

				asd.Engine.Tool.Columns(1);

				asd.Engine.Tool.End();
			}


		}

		void Terminate()
		{
			if (psd != null)
			{
				psd.Terminate();
			}
			psd = null;
		}

		void Open()
		{
			var result = asd.Engine.Tool.OpenDialog("psd", System.IO.Directory.GetCurrentDirectory());
			if(!string.IsNullOrEmpty(result))
			{
				Load(result);
				selectedItem = -1;
			}
		}

		void Save()
		{
			var result = asd.Engine.Tool.SaveDialog("aip", System.IO.Directory.GetCurrentDirectory());
			if (!string.IsNullOrEmpty(result))
			{
				if (result.EndsWith(".aip")) result += ".aip";
				System.IO.File.WriteAllBytes(result, Export());
			}
		}

		byte[] Export()
		{
			return psd.Export();
		}

		public void Load(string path)
		{
			foreach (var layer in psdLayers)
			{
				layer.Dispose();
			}
			psdLayers.Clear();

			var ec = psd.Load(path);

			if (ec == ErrorCode.DepthIs16)
			{
				asd.Engine.Tool.ShowDialog("8bit画像のみサポートしています。\npsdファイルのチャンネルを確認するようお願いします。", "Info", asd.ToolDialogStyle.Info, asd.ToolDialogButtons.OK);
				return;
			}
			else if (ec != ErrorCode.Succeed)
			{
				asd.Engine.Tool.ShowDialog("読込に失敗しました。", "Info", asd.ToolDialogStyle.Error, asd.ToolDialogButtons.OK);
				return;
			}

			foreach (var l in psd.Layers)
			{
				psdLayers.Add(new Layer(l));
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

				var wi = asd.Engine.Graphics.CreateEmptyTexture2D(Rect.Width, Rect.Height, asd.TextureFormat.R8G8B8A8_UNORM_SRGB);

				fixed (Color* p = layer.Pixels)
				{
					asd.TextureLockInfomation info = new asd.TextureLockInfomation();
					wi.Lock(info);

					for (int y = 0; y < Rect.Height; y++)
					{
						for (int x = 0; x < Rect.Width; x++)
						{
							var src = *(p + x + y * Rect.Width);
							var dst = (Color*)info.Pixels + x + y * Rect.Width;

							var r = src.B;
							var b = src.R;
							src.B = r;
							src.R = b;

							*dst = src;
						}
					}

					wi.Unlock();
				}

				Image = wi;
			}

			public string Name { get; private set; }

			public Rect Rect { get; private set; }

			public asd.Texture2D Image { get; private set; }

			public void Dispose()
			{
				if (Image != null)
				{
					Image.ForceToRelease();
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
