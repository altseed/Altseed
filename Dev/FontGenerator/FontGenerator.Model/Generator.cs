using System;
using System.Collections.Generic;
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
			var gen = new DLL();
			gen.SetFontName(config.FontPath);
			gen.SetTextFilePath(config.TextPath);
			gen.SetExportPath(config.ExportPath);
			gen.SetFontSize(config.FontSize);

			var c = config.FontColor;
			gen.SetFontColor(c.Red, c.Green, c.Blue, c.Alpha);

			await Task.Run(() => gen.Run());
		}
	}
}
