using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Altseed
{
	class StringConverter
	{
		public string SbyteToString(sbyte[] str)
		{
			return Encoding.UTF8.GetString(str.Select(x => (byte)x).ToArray());
		}

		public sbyte[] StringToSbyte(string str)
		{
			return Encoding.UTF8.GetBytes(str).Select(x => (sbyte)x).ToArray();
		}
	}
}
