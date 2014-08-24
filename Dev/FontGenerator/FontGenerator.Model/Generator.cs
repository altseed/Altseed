using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FontGenerator.swig;

namespace FontGenerator.Model
{
	public class Generator
	{
		public static async Task GenerateAsync(GenerationConfig config)
		{
			if(!File.Exists(config.FontPath))
			{
				throw new FileNotFoundException("指定されたフォントファイルは存在しません。", config.FontPath);
			}

			if(!File.Exists(config.TextPath))
			{
				throw new FileNotFoundException("指定されたテキストファイルは存在しません。", config.TextPath);
			}

			if(!Directory.Exists(config.ExportPath))
			{
				throw new FileNotFoundException("指定され出力先ディレクトリは存在しません。", config.ExportPath);
			}

			var gen = new DLL();
			gen.SetFontName(config.FontPath);
			gen.SetTextFilePath(config.TextPath);
			gen.SetExportPath(Path.Combine(config.ExportPath, config.SheetName));
			gen.SetFontSize(config.FontSize);
			gen.SetSheetSize(config.TextureSize);

			var c = config.FontColor;
			gen.SetFontColor(c.Red, c.Green, c.Blue, c.Alpha);

			var co = config.OutlineColor;
			gen.SetOutlineColor(co.Red, co.Green, co.Blue, co.Alpha);
			gen.SetOutlineSize(config.OutlineSize);
			gen.SetOutlineSampling(config.OutlineSampling);

			await Task.Run(() => gen.Run());
		}
	}
}
