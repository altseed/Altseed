using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	public class GenerationConfig
	{
		public string FontPath { get; set; }
		public string TextPath { get; set; }
		public string ExportPath { get; set; }
		public int FontSize { get; set; }
		public Color FontColor { get; set; }
	}
}
