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
	public abstract class SceneComponent : Component
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
		public Scene Owner { get; internal set; }

		/// <summary>
		/// このコンポーネントが更新されるかどうかを取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの Update が始まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの Update が終わる時の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdated()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンがエンジンに追加された時の処理を記述できる。
		/// </summary>
		protected virtual void OnRegistered()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの更新が始まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnStartUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンへの画面遷移が完了した時の処理を記述できる。
		/// </summary>
		protected virtual void OnTransitionFinished()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンからの画面遷移が始まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnTransitionBegin()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの更新が止まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnStopUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンがエンジンから登録解除された時の処理を記述できる。
		/// </summary>
		protected virtual void OnUnregistered()
		{
		}

		internal void RaiseOnUpdating()
		{
			if(IsUpdated && IsAlive)
			{
				OnUpdating();
			}
		}

		internal void RaiseOnUpdated()
		{
			if (IsUpdated && IsAlive)
			{
				OnUpdated();
			}
		}

		internal void RaiseOnRegistered()
		{
			if(IsAlive)
			{
				OnRegistered();
			}
		}

		internal void RaiseOnStartUpdating()
		{
			if(IsAlive)
			{
				OnStartUpdating();
			}
		}

		internal void RaiseOnTransitionFinished()
		{
			if(IsAlive)
			{
				OnTransitionFinished();
			}
		}

		internal void RaiseOnTransitionBegin()
		{
			if(IsAlive)
			{
				OnTransitionBegin();
			}
		}

		internal void RaiseOnStopUpdating()
		{
			if(IsAlive)
			{
				OnStopUpdating();
			}
		}

		internal void RaiseOnUnregistered()
		{
			if(IsAlive)
			{
				OnUnregistered();
			}
		}

		internal override void ImmediatelyDispose()
		{
			IsAlive = false;
			Owner.ImmediatelyRemoveComponent(Key);
		}
	}
}
