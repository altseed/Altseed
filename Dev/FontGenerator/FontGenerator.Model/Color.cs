using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	[DataContract]
	public struct Color
	{
		public Color(byte r, byte g, byte b, byte a) : this()
		{
			this.Red = r;
			this.Green = g;
			this.Blue = b;
			this.Alpha = a;
		}

		[DataMember]
		public byte Red { get; set; }
		[DataMember]
		public byte Green { get; set; }
		[DataMember]
		public byte Blue { get; set; }
		[DataMember]
		public byte Alpha { get; set; }
	}
}
