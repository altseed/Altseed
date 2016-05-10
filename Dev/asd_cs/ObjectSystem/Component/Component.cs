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
	public abstract class Component
	{
		/// <summary>
		/// このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		/// </summary>
		public bool IsAlive { get; internal set; }

		public string Key { get; internal set; }

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
		public void Dispose()
		{
			Engine.ChangesToBeCommited.Enqueue(new EventToDisposeComponent(this));
		}

		internal abstract void ImmediatelyDispose();
	}
}
