using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class ToolRenderer
	{
		private Model.GenerationConfig config;
		private ConfigViewModel viewModel;

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
							tool.SetItemDefaultFocus();
							break;
						}
					}
					tool.EndCombo();
				}

				//tool.InputInt("Font Index", ref viewModel.FontIndex);
				//tool.InputText("Text File", viewModel.TextPath, ConfigViewModel.TextSize);
				//tool.InputText("Export Directory", viewModel.ExportPath, ConfigViewModel.TextSize);
				//tool.InputText("Sheet Name", viewModel.SheetName, ConfigViewModel.TextSize);

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
