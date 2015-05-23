using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public partial class Joystick
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="js">ネイティブインタフェース</param>
		internal Joystick(ace.swig.Joystick js)
		{
			CoreInstance = js;
		}
	}
}
