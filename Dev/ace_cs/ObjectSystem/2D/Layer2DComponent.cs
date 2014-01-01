using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// ace.Layer2D クラスに登録することができるコンポーネント クラス。
	/// </summary>
	public abstract class Layer2DComponent
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Layer2DComponent()
		{
			IsUpdated = true;
			IsAlive = true;
		}

		/// <summary>
		/// このコンポーネントを保持しているレイヤー クラスのインスタンスを取得する。
		/// </summary>
		public Layer2D Owner { get; internal set; }
		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得する。
		/// </summary>
		public bool IsUpdated { get; set; }
		/// <summary>
		/// このコンポーネントが実行中かどうかを取得する。Vanishメソッドによって破棄された時に false を返す。
		/// </summary>
		public bool IsAlive { get; private set; }

		/// <summary>
		/// このコンポーネントを破棄する。
		/// </summary>
		public void Vanish()
		{
			IsAlive = false;
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントの Update時の処理を記述できる。
		/// </summary>
		protected abstract void OnUpdated();

		internal void Update()
		{
			if( IsAlive && IsUpdated )
			{
				OnUpdated();
			}
		}
	}
}
