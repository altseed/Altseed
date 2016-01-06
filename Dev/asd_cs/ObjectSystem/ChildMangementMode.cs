using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 子オブジェクトの管理に関する同期の設定フラグ。
	/// </summary>
	[Flags]
	public enum ChildManagementMode : int
	{
		/// <summary>
		/// 何も同期しない。
		/// </summary>
		Nothing = 0,
		/// <summary>
		/// レイヤーへの登録状況を同期する。
		/// </summary>
		RegistrationToLayer = 1,
		/// <summary>
		/// 親オブジェクトが破棄されたとき、子オブジェクトも破棄する。
		/// </summary>
		Vanishment = 2,
		/// <summary>
		/// 親オブジェクトが更新されないとき、子オブジェクトも更新しない。
		/// </summary>
		IsUpdated = 4,
		/// <summary>
		/// 親オブジェクトが描画されないとき、子オブジェクトも描画されない。
		/// </summary>
		IsDrawn = 8,
	}
}
