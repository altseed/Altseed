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
				tool.InputInt("Font Index", ref viewModel.FontIndex);
				tool.InputText("Text File", viewModel.TextPath, ConfigViewModel.TextSize);
				tool.InputText("Export Directory", viewModel.ExportPath, ConfigViewModel.TextSize);
				tool.InputText("Sheet Name", viewModel.SheetName, ConfigViewModel.TextSize);
			}

			tool.End();
		}
	}
}
