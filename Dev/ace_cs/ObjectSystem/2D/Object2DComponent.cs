using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// ace.Object2D クラスに登録できるコンポーネント クラス。
	/// </summary>
	public abstract class Object2DComponent
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Object2DComponent()
		{
			IsUpdated = true;
			IsAlive = true;
		}

		/// <summary>
		/// このコンポーネントを保持している2Dオブジェクトのインスタンスを取得する。
		/// </summary>
		public Object2D Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得または設定する。
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
		protected abstract void OnUpdate();

		internal void Update()
		{
			if( IsUpdated && IsAlive )
			{
				OnUpdate();
			}
		}
	}
}
