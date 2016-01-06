using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// 子オブジェクトの描画に関する同期の設定を表すフラグ。
	/// </summary>
	[Flags]
	public enum ChildDrawingMode : int
	{
		/// <summary>
		/// 何も同期しない。
		/// </summary>
		Nothing = 0,
		/// <summary>
		/// 描画の際に親の色を子に乗算する。
		/// </summary>
		Color = 1,
		/// <summary>
		/// 描画の際に親の描画優先度を子の描画優先度に足す。
		/// </summary>
		DrawingPriority = 2,
	}
}
