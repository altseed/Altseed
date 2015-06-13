using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 破棄可能なインスタンス
	/// </summary>
	public interface IDestroy
	{
		/// <summary>
		/// 破棄されているか？
		/// </summary>
		bool IsDestroyed {get;}

		/// <summary>
		/// 破棄する。
		/// </summary>
		void Destroy();
	}
}
