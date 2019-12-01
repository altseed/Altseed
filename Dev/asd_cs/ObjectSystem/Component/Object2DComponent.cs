using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace asd
{
	/// <summary>
	/// asd.Object2D クラスに登録できるコンポーネント クラス。
	/// </summary>
	public abstract class Object2DComponent : Component
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
		public Object2D Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つオブジェクトが更新される時の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdate()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つオブジェクトがレイヤーに登録された時の処理を記述できる。
		/// </summary>
		protected virtual void OnObjectAdded()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つオブジェクトがレイヤーから登録解除された時の処理を記述できる。
		/// </summary>
		protected virtual void OnObjectRemoved()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つオブジェクトが破棄された時の処理を記述できる。
		/// </summary>
		protected virtual void OnObjectDisposed()
		{
		}

		internal void RaiseOnUpdate()
		{
			if( IsUpdated && IsAlive )
			{
				OnUpdate();
			}
		}

		internal void RaiseOnAdded()
		{
			if (IsAlive)
			{
				OnObjectAdded();
			}
		}

		internal void RaiseOnRemoved()
		{
			if (IsAlive)
			{
				OnObjectRemoved();
			}
		}

		internal void RaiseOnDisposed()
		{
			if (IsAlive)
			{
				OnObjectDisposed();
			}
		}

		internal override void ImmediatelyDispose()
		{
			IsAlive = false;
			Owner?.ImmediatelyRemoveComponent(Key);
		}
	}
}
