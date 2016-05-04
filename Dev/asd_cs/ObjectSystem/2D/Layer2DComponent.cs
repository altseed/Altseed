using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// asd.Layer2D クラスに登録することができるコンポーネント クラス。
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
		/// オーバーライドして、このコンポーネントを持つレイヤーが更新される直前の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つレイヤーが更新された直後の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdated()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つレイヤーがシーンに登録された時の処理を記述できる。
		/// </summary>
		protected virtual void OnAdded()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つレイヤーがシーンから登録解除された時の処理を記述できる。
		/// </summary>
		protected virtual void OnRemoved()
		{
		}

		internal void RaiseOnUpdating()
		{
			if (IsAlive && IsUpdated)
			{
				OnUpdating();
			}
		}

		internal void RaiseOnUpdated()
		{
			if( IsAlive && IsUpdated )
			{
				OnUpdated();
			}
		}

		internal void RaiesOnAdded()
		{
			if (IsAlive)
			{
				OnAdded();
			}
		}

		internal void RaiesOnRemoved()
		{
			if (IsAlive)
			{
				OnRemoved();
			}
		}
	}
}
