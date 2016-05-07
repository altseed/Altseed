using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// コンポーネント クラスの基底クラス。
	/// </summary>
	/// <typeparam name="TOwner">コンポーネントの追加先とできる型。</typeparam>
	public abstract class Component<TOwner> where TOwner : class
	{
		/// <summary>
		/// このコンポーネントを保持しているインスタンスを取得する。
		/// </summary>
		public abstract TOwner Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		/// </summary>
		public bool IsAlive { get; private set; }

		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Component()
		{
			IsAlive = true;
		}

		/// <summary>
		/// このコンポーネントを破棄する。
		/// </summary>
		public void Vanish()
		{
			IsAlive = false;
		}
	}
}
