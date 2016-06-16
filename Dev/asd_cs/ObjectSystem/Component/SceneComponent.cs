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
		protected virtual void OnSceneRegistered()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの更新が始まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnStartSceneUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンへの画面遷移が完了した時の処理を記述できる。
		/// </summary>
		protected virtual void OnSceneTransitionFinished()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンからの画面遷移が始まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnSceneTransitionBegin()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンの更新が止まる時の処理を記述できる。
		/// </summary>
		protected virtual void OnStopSceneUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンがエンジンから登録解除された時の処理を記述できる。
		/// </summary>
		protected virtual void OnSceneUnregistered()
		{
		}

		/// <summary>
		/// オーバーライドして、このコンポーネントを持つシーンが破棄された時の処理を記述できる。
		/// </summary>
		protected virtual void OnSceneDisposed()
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
				OnSceneRegistered();
			}
		}

		internal void RaiseOnStartUpdating()
		{
			if(IsAlive)
			{
				OnStartSceneUpdating();
			}
		}

		internal void RaiseOnTransitionFinished()
		{
			if(IsAlive)
			{
				OnSceneTransitionFinished();
			}
		}

		internal void RaiseOnTransitionBegin()
		{
			if(IsAlive)
			{
				OnSceneTransitionBegin();
			}
		}

		internal void RaiseOnStopUpdating()
		{
			if(IsAlive)
			{
				OnStopSceneUpdating();
			}
		}

		internal void RaiseOnUnregistered()
		{
			if(IsAlive)
			{
				OnSceneUnregistered();
			}
		}

		internal void RaiseOnDisposed()
		{
			if (IsAlive)
			{
				OnSceneDisposed();
			}
		}

		internal override void ImmediatelyDispose()
		{
			IsAlive = false;
			Owner.ImmediatelyRemoveComponent(Key);
		}
	}
}
