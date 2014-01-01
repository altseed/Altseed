using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// ace.Scene クラスに登録することができるコンポーネント。
	/// </summary>
	public abstract class SceneComponent
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public SceneComponent()
		{
			IsUpdated = true;
			IsAlive = true;
		}

		/// <summary>
		/// このインスタンスを保持しているシーン クラスを取得する。
		/// </summary>
		public Scene Owner { get; internal set; }
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
		protected abstract void OnUpdated();

		internal void Update()
		{
			if( IsUpdated && IsAlive )
			{
				OnUpdated();
			}
		}
	}
}
