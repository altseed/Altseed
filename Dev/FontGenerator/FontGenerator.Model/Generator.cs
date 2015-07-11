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
			var pathes = Helper.GetFontPairs()
				.Select(x => x.Path)
				.ToArray();

			var path = pathes[config.FontIndex];

			if(!File.Exists(path))
			{
				throw new FileNotFoundException("指定されたフォントファイルは存在しません。", path);
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
			gen.SetFontName(path);
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

		public static async Task<string> GeneratePreviewAsync(GenerationConfig config)
		{
			var pathes = Helper.GetFontPairs()
				.Select(x => x.Path)
				.ToArray();

			var path = pathes[config.FontIndex];

			var gen = new DLL();
			gen.SetFontName(path);
			gen.SetFontSize(config.FontSize);

			var c = config.FontColor;
			gen.SetFontColor(c.Red, c.Green, c.Blue, c.Alpha);

			var co = config.OutlineColor;
			gen.SetOutlineColor(co.Red, co.Green, co.Blue, co.Alpha);
			gen.SetOutlineSize(config.OutlineSize);
			gen.SetOutlineSampling(config.OutlineSampling);

			return await Task.Run(() => gen.SavePreview());
		}
	}
}
