using FontGenerator.Model;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class ToolRenderer
	{
		private ConfigViewModel viewModel;
		private ConfigConverter converter;
		private StringConverter strConverter;

		int selectedFont = 0;
		string fontListStr = string.Empty;
		List<Model.FontPair> fontPairs = new List<Model.FontPair>();

		public ToolRenderer()
		{
			TryLoadFont(
				new[] {
					"C:\\Windows\\Fonts\\meiryo.ttc",
					"/Library/Fonts/ヒラギノ丸ゴ Pro W4.otf",
					"/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc",
					"/System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc",
				}
				);

			var name2path = Model.Helper.GetFontPairs();

			foreach (var np in name2path)
			{
				fontPairs.Add(np);
			}

			fontListStr = string.Join(";", fontPairs.Select(_ => _.Name));

			viewModel = new ConfigViewModel();
			strConverter = new StringConverter();
			converter = new ConfigConverter(strConverter);

			viewModel.FontName = fontPairs[selectedFont].Name;
		}

		public void Render()
		{
			var tool = asd.Engine.Tool;

			if (asd.Engine.Tool.BeginFullscreen("FontGenerator", 0))
			{
				var fontType = new[] { selectedFont };

				if (tool.BeginCombo("フォントの種類", fontPairs[selectedFont].Name))
				{
					for(int i = 0; i < fontPairs.Count; i++)
					{
						if(tool.Selectable(fontPairs[i].Name, i == selectedFont))
						{
							selectedFont = i;
							viewModel.FontName = fontPairs[i].Name;
							tool.SetItemDefaultFocus();
							break;
						}
					}
					tool.EndCombo();
				}

				tool.InputText("文字一覧ファイル", viewModel.TextPath, ConfigViewModel.TextSize);
				if (tool.Button("ファイルを開く..."))
				{
					var path = tool.OpenDialog("txt", Directory.GetCurrentDirectory());
					var bytes = strConverter.StringToSbyte(path);
					for (int i = 0; i < ConfigViewModel.TextSize; i++)
					{
						viewModel.TextPath[i] = i < bytes.Length ? bytes[i] : (sbyte)0;
					}
				}

				tool.InputText("出力先ディレクトリ", viewModel.ExportPath, ConfigViewModel.TextSize);
				if (tool.Button("ディレクトリを開く..."))
				{
					var path = tool.PickFolder(Directory.GetCurrentDirectory());
					var bytes = strConverter.StringToSbyte(path);
					for (int i = 0; i < ConfigViewModel.TextSize; i++)
					{
						viewModel.ExportPath[i] = i < bytes.Length ? bytes[i] : (sbyte)0;
					}
				}

				tool.InputText("シート名", viewModel.SheetName, ConfigViewModel.TextSize);
				tool.InputInt("フォントサイズ", ref viewModel.FontSize);
				tool.InputInt("テクスチャサイズ", ref viewModel.TextureSize);
				tool.InputInt("輪郭線の太さ", ref viewModel.OutlineSize);
				tool.Color("フォント色", ref viewModel.FontColor);
				tool.Color("輪郭線の色", ref viewModel.OutlineColor);

				if (tool.Button("設定ロード..."))
				{
					var path = tool.OpenDialog("afcfg", Directory.GetCurrentDirectory());
					viewModel = converter.LoadFromModel(ConfigurationFile.Load(path));
					selectedFont = fontPairs.IndexOf(fontPairs.Find(x => x.Name == viewModel.FontName));
				}
				if (tool.Button("設定セーブ..."))
				{
					var path = tool.SaveDialog("afcfg", Directory.GetCurrentDirectory());
					ConfigurationFile.Save(converter.ConvertToModel(viewModel), path);
				}
				tool.Button("aff生成");

				tool.End();
			}
		}

		static void TryLoadFont(string[] fonts)
		{
			foreach (var font in fonts)
			{
				if (!System.IO.File.Exists(font)) continue;
				asd.Engine.Tool.AddFontFromFileTTF(font, 14);
				break;
			}
		}

	}
}
