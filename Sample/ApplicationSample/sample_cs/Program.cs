using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace sample_cs
{
	class Program
	{
		static void Main(string[] args)
		{
			ISample[] samples =
			{
				new ImagePackageUI_Basic(),
				new ImagePackageUI_AlphaBlend(),
				new ImagePackageUI_Component(),
            };

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}
