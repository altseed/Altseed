using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class ConfigConverter
	{
		private StringConverter strConverter;

		public ConfigConverter(StringConverter strConverter)
		{
			this.strConverter = strConverter;
		}

		private Model.Color ToModelColor(asd.Color color)
		{
			return new Model.Color(color.R, color.G, color.B, color.A);
		}

		private asd.Color ToAsdColor(Model.Color color)
		{
			return new asd.Color(color.Red, color.Green, color.Blue, color.Alpha);
		}

		public Model.GenerationConfig ConvertToModel(ConfigViewModel viewModel)
		{
			return new Model.GenerationConfig
			{
				FontIndex = viewModel.FontIndex.Value,
				FontName = viewModel.FontName.Value,
				TextPath = viewModel.TextPath.String,
				ExportPath = viewModel.ExportPath.String,
				SheetName = viewModel.SheetName.String,
				FontSize = viewModel.FontSize.Value,
				TextureSize = viewModel.TextureSize.Value,
				OutlineSize = viewModel.OutlineSize.Value,
				OutlineSampling = viewModel.OutlineSampling.Value,
				FontColor = ToModelColor(viewModel.FontColor.Value),
				OutlineColor = ToModelColor(viewModel.OutlineColor.Value),
			};
		}

		public ConfigViewModel LoadFromModel(Model.GenerationConfig config)
		{
			return new ConfigViewModel
			{
				FontName = { Value = config.FontName },
				TextPath = { String = config.TextPath },
				ExportPath = { String = config.ExportPath },
				SheetName = { String = config.SheetName },
				FontSize = { Value = config.FontSize },
				TextureSize = { Value = config.TextureSize },
				OutlineSize = { Value = config.OutlineSize },
				OutlineSampling = { Value = config.OutlineSampling },
				FontColor = { Value = ToAsdColor(config.FontColor) },
				OutlineColor = { Value = ToAsdColor(config.OutlineColor) },
			};
		}
	}
}
