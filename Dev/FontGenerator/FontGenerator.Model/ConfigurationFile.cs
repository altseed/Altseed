using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;

namespace FontGenerator.Model
{
	public class ConfigurationFile
	{
		public static void Save(GenerationConfig config, string path)
		{
			var serializer = new DataContractJsonSerializer(typeof(GenerationConfig));
			using (var file = File.Create(path))
			{
				serializer.WriteObject(file, config);
			}
		}

		public static GenerationConfig Load(string path)
		{
			var serializer = new DataContractJsonSerializer(typeof(GenerationConfig));
			GenerationConfig result;
			using (var file = File.OpenRead(path))
			{
				result = serializer.ReadObject(file) as GenerationConfig;
			}

			var fontPairs = Helper.GetFontPairs();
			for (int i = 0; i < fontPairs.Count; i++)
			{
				if (result.FontName == fontPairs[i].Name)
				{
					result.FontIndex = i;
				}
			}

			return result;
		}
	}
}
