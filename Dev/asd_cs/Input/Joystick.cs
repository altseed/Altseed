using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Joystick
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="js">ネイティブインタフェース</param>
		internal Joystick(asd.swig.Joystick js)
		{
			CoreInstance = js;
		}
	}
}
