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

		public void Render()
		{
			var tool = asd.Engine.Tool;

			tool.Begin("FontGenerator");

			{
				tool.InputInt("フォントインデックス", ref viewModel.FontIndex);
				tool.InputText("テキストファイル", viewModel.TextPath, ConfigViewModel.TextSize);
				tool.InputText("出力先ディレクトリ", viewModel.ExportPath, ConfigViewModel.TextSize);
				tool.InputText("シート名", viewModel.SheetName, ConfigViewModel.TextSize);
			}

			tool.End();
		}
	}
}
