using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	public partial class Tool
	{
		/// <summary>
		/// Tool のインスタンスを生成します。
		/// </summary>
		/// <param name="coreInstance">Core側のインスタンス。</param>
		internal Tool(swig.Tool coreInstance)
		{
			CoreInstance = coreInstance;
		}
	}
}
