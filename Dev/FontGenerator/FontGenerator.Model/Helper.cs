using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	public class FontPair
	{
		public string Name = string.Empty;
		public string Path = string.Empty;

		public FontPair(string name, string path)
		{
			Name = name;
			Path = path;
		}

		public override string ToString()
		{
			return Name;
		}
	}

	public class Helper
	{
		public static Dictionary<string, string> GetFontNameToPathDictonary()
		{
			var ret = new Dictionary<string, string>();
			var dll = new swig.DLL();
			dll.LoadFontNames();

			for(var i = 0; i < dll.GetFontCount(); i++)
			{
				var name = dll.GetFontName(i);
				var path = dll.GetFontPath(i);

				ret.Add(name, path);
			}

			return ret;
		}

		public static List<FontPair> GetFontPairs()
		{
			var ret = new List<FontPair>();
			var dll = new swig.DLL();
			dll.LoadFontNames();

			for(var i = 0; i < dll.GetFontCount(); i++)
			{
				var name = dll.GetFontName(i);
				var path = dll.GetFontPath(i);

				ret.Add(new FontPair(name, path));
			}

			ret.Sort((x, y) => string.Compare(x.Name, y.Name));

			return ret;
		}
	}
}
