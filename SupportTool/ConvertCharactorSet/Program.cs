using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConvertCharactorSet
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Length < 3) return;

			var files = System.IO.Directory.GetFiles(args[0], args[1], System.IO.SearchOption.AllDirectories);

			EncodingType to = EncodingType.UTF8;

			if (args[2] == "UTF8") to = EncodingType.UTF8;
			if (args[2] == "CP932") to = EncodingType.CP932;

			if (System.IO.File.Exists(args[0]))
			{
				Convert(args[0], to);
			}
			else
			{
				foreach (var file in files)
				{
					Convert(file, to);
				}
			}
		}

		static void Convert(string path, EncodingType to)
		{
			var binary = System.IO.File.ReadAllBytes(path);

			if (binary.Length < 3) return;

			EncodingType type = EncodingType.CP932;

			if ( binary[0] == 0xEF &&
				 binary[1] == 0xBB && 
				 binary[2] == 0xBF)
			{
				type = EncodingType.UTF8;
			}

			string text = null;

			if (type == EncodingType.UTF8)
			{
				text = System.IO.File.ReadAllText(path, Encoding.UTF8);
			}
			else if (type == EncodingType.CP932)
			{
				text = System.IO.File.ReadAllText(path, Encoding.GetEncoding(932));
			}

			if (to == EncodingType.UTF8)
			{
				System.IO.File.WriteAllText(path, text, Encoding.UTF8);
			}
			else if(to == EncodingType.CP932)
			{
				System.IO.File.WriteAllText(path, text, Encoding.GetEncoding(932));
			}
		}

		enum EncodingType
		{ 
			UTF8,
			CP932,
		}
	}
}
