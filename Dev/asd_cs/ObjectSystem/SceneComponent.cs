using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// asd.Scene クラスに登録することができるコンポーネント。
	/// </summary>
	public abstract class SceneComponent : Component<Scene>
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public SceneComponent()
		{
			IsUpdated = true;
		}

		/// <summary>
		/// このインスタンスを保持しているシーン クラスを取得する。
		/// </summary>
		public override Scene Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オーバーライドして、このコンポーネントの Update時の処理を記述できる。
		/// </summary>
		protected abstract void OnUpdated();

		internal override void Update()
		{
			if( IsUpdated && IsAlive )
			{
				OnUpdated();
			}
		}
	}
}
