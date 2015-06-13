using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public class RenderTexture2D : Texture2D
	{
		internal RenderTexture2D(swig.RenderTexture2D swig)
			: base(swig)
		{
			SwigObject = swig;
		}
	}
}
