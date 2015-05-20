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
	public abstract class Layer2DComponent : Component<Layer2D>
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Layer2DComponent()
		{
			IsUpdated = true;
		}

		/// <summary>
		/// このコンポーネントを保持しているレイヤー クラスのインスタンスを取得する。
		/// </summary>
		public override Layer2D Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オーバーライドして、このコンポーネントの Update時の処理を記述できる。
		/// </summary>
		protected abstract void OnUpdated();

		internal override void Update()
		{
			if( IsAlive && IsUpdated )
			{
				OnUpdated();
			}
		}
	}
}
