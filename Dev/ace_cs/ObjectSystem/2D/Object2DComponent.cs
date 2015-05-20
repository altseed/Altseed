using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// ace.Object2D クラスに登録できるコンポーネント クラス。
	/// </summary>
	public abstract class Object2DComponent : Component<Object2D>
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Object2DComponent()
		{
			IsUpdated = true;
		}

		/// <summary>
		/// このコンポーネントを保持している2Dオブジェクトのインスタンスを取得する。
		/// </summary>
		public override Object2D Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オーバーライドして、このコンポーネントの Update時の処理を記述できる。
		/// </summary>
		protected abstract void OnUpdate();

		internal override void Update()
		{
			if( IsUpdated && IsAlive )
			{
				OnUpdate();
			}
		}
	}
}
