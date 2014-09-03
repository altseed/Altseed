using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
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
	}
}
