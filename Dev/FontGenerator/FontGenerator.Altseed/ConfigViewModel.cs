using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class ConfigViewModel
	{
		public static readonly int TextSize = 2048;

		public int FontIndex = 0;
		public string FontName = "";
		public sbyte[] TextPath = new sbyte[TextSize];
		public sbyte[] ExportPath = new sbyte[TextSize];
		public sbyte[] SheetName = new sbyte[TextSize];
		public int FontSize = 1;
		public int TextureSize = 256;
		public int OutlineSize = 0;
		public int OutlineSampling = 1;
		public asd.Color FontColor = new asd.Color(255, 255, 255, 255);
		public asd.Color OutlineColor = new asd.Color(0, 0, 0, 255);
	}
}
