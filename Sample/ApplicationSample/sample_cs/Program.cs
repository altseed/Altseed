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

				new Pause_Basic(),

				new CustomPostEffect_Invert(),
				new CustomPostEffect_Mosaic(),

                new MapObject2D_Basic(),
                new MapObject2D_Camera(),

				new Fps_ShowFps(),
            };

			var cui = new SampleGuide( samples );
			cui.Show();
		}
	}
}
