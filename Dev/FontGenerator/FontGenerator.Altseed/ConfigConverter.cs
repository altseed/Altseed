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
				FontName = viewModel.FontName,
				TextPath = strConverter.SbyteToString(viewModel.TextPath),
				ExportPath = strConverter.SbyteToString(viewModel.ExportPath),
				SheetName = strConverter.SbyteToString(viewModel.SheetName),
				FontSize = viewModel.FontSize,
				TextureSize = viewModel.TextureSize,
				OutlineSize = viewModel.OutlineSize,
				OutlineSampling = viewModel.OutlineSampling,
				FontColor = ToModelColor(viewModel.FontColor),
				OutlineColor = ToModelColor(viewModel.OutlineColor),
			};
		}

		public ConfigViewModel LoadFromModel(Model.GenerationConfig config)
		{
			return new ConfigViewModel
			{
				FontName = config.FontName,
				TextPath = strConverter.StringToSbyte(config.TextPath),
				ExportPath = strConverter.StringToSbyte(config.ExportPath),
				SheetName = strConverter.StringToSbyte(config.SheetName),
				FontSize = config.FontSize,
				TextureSize = config.TextureSize,
				OutlineSize = config.OutlineSize,
				OutlineSampling = config.OutlineSampling,
				FontColor = ToAsdColor(config.FontColor),
				OutlineColor = ToAsdColor(config.OutlineColor),
			};
		}
	}
}
