using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	[DataContract]
	public class GenerationConfig
	{
		[DataMember]
		public string FontPath { get; set; }
		[DataMember]
		public string TextPath { get; set; }
		[DataMember]
		public string ExportPath { get; set; }
		[DataMember]
		public string SheetName { get; set; }
		[DataMember]
		public int FontSize { get; set; }
		[DataMember]
		public int TextureSize { get; set; }
		[DataMember]
		public Color FontColor;
	}
}
