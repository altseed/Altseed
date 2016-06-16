using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
	/// <summary>
	/// レイヤーの更新と描画を管理するシーン機能を提供するクラス。
	/// </summary>
	public class Scene : IReleasable, IDisposable, IBeingAbleToDisposeNative, IComponentRegisterable<SceneComponent>
	{
		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Scene()
		{
			CoreInstance = Engine.ObjectSystemFactory.CreateScene();

			var p = CoreInstance.GetPtr();

			if(GC.Scenes.Contains(p))
			{
				Particular.Helper.ThrowException("");
			}

			GC.Scenes.AddObject(p, this);

			layersToDraw_ = new List<Layer>();
			layersToUpdate_ = new List<Layer>();
			ComponentManager = new ComponentManager<SceneComponent>(this);

			IsAlive = true;
		}

		#region GC対策
		~Scene()
		{
			ForceToRelease();
		}

		public bool IsReleased
		{
			get
			{
				return CoreInstance == null;
			}
		}

		public void ForceToRelease()
		{
			lock (this)
			{
				if(CoreInstance == null) return;
				GC.Collector.AddObject(CoreInstance);
				CoreInstance = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion

		internal void ThrowIfDisposed()
		{
			if(!IsAlive)
			{
				throw new ObjectDisposedException(GetType().FullName);
			}
		}


		/// <summary>
		/// このシーンが有効かどうかの真偽値を取得する。破棄されていれば false を返す。
		/// </summary>
		public bool IsAlive { get; private set; }

		/// <summary>
		/// 描画先がHDRかどうか、取得、または設定する。
		/// </summary>
		public bool HDRMode
		{
			get { return CoreInstance.GetHDRMode(); }
			set { CoreInstance.SetHDRMode(value); }
		}

		/// <summary>
		/// 全てのレイヤーとポストエフェクトが描画され終わった画面をテクスチャとして取得する。
		/// </summary>
		/// <returns>画面</returns>
		/// <remarks>テクスチャの内容はシーンが描画されるたびに変わる。主にシーン遷移の際に使用する。</remarks>
		public RenderTexture2D EffectedScreen
		{
			get
			{
				return GC.GenerateRenderTexture2D(CoreInstance.GetBaseTarget(), GenerationType.Get);
			}
		}

		/// <summary>
		/// 所属しているレイヤーを取得する。
		/// </summary>
		public IEnumerable<Layer> Layers { get { return layersToUpdate_; } }

		#region 管理
		/// <summary>
		/// 指定したレイヤーをこのシーンに追加する。
		/// </summary>
		/// <param name="layer">追加されるレイヤー</param>
		/// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void AddLayer(Layer layer)
		{
			if(layer.Scene != null)
			{
				throw new InvalidOperationException("指定したレイヤーは、既に別のシーンに所属しています。");
			}

			Engine.ChangesToBeCommited.Enqueue(new EventToManageLayer(this, layer, RegistrationCommand.Add, true));
			layer.Scene = this;
		}

		/// <summary>
		/// 指定したレイヤーをこのシーンから削除する。
		/// </summary>
		/// <param name="layer">削除されるレイヤー</param>
		/// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void RemoveLayer(Layer layer)
		{
			Engine.ChangesToBeCommited.Enqueue(new EventToManageLayer(this, layer, RegistrationCommand.Remove, true));
			layer.Scene = null;
		}

		/// <summary>
		/// 指定したコンポーネントをこのシーンに追加する。
		/// </summary>
		/// <param name="component">追加するコンポーネント</param>
		/// <param name="key">コンポーネントに関連付けるキー</param>
		/// <remarks>実際に追加されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public void AddComponent(SceneComponent component, string key)
		{
			ComponentManager.Add(component, key);
		}

		/// <summary>
		/// 指定したコンポーネントをこのシーンから削除する。
		/// </summary>
		/// <param name="key">削除するコンポーネントを示すキー</param>
		/// <remarks>実際に削除されるのはこのメソッドを呼び出したフレームの最後になるので注意が必要。</remarks>
		public bool RemoveComponent(string key)
		{
			return ComponentManager.Remove(key);
		}

		/// <summary>
		/// 指定したキーを持つコンポーネントを取得する。
		/// </summary>
		/// <param name="key">取得するコンポーネントのキー</param>
		/// <returns>コンポーネント</returns>
		public SceneComponent GetComponent(string key)
		{
			return ComponentManager.Get(key);
		}

		/// <summary>
		/// このシーンを破棄する。
		/// </summary>
		/// <remarks>登録されているレイヤーもすべて破棄されるが、レイヤーの破棄はこのメソッドを呼んだフレームの最後に実行されるので注意が必要。</remarks>
		public void Dispose()
		{
			Dispose(false);
		}

		/// <summary>
		/// このシーンを破棄する。
		/// </summary>
		/// <param name="disposeNative">ネイティブ リソースも即破棄するかどうかの真偽値。</param>
		/// <remarks>登録されているレイヤーもすべて破棄されるが、レイヤーの破棄はこのメソッドを呼んだフレームの最後に実行されるので注意が必要。</remarks>
		public void Dispose(bool disposeNative)
		{
			Engine.ChangesToBeCommited.Enqueue(new EventToDisposeContent(this, disposeNative));
		}

		internal void ImmediatelyAddLayer(Layer layer, bool raiseEvent)
		{
			layersToDraw_.Add(layer);
			layersToUpdate_.Add(layer);
			CoreInstance.AddLayer(layer.CoreLayer);
			if(raiseEvent)
			{
				layer.RaiseOnAdded();
			}
		}

		internal void ImmediatelyRemoveLayer(Layer layer, bool raiseEvent)
		{
			if(raiseEvent)
			{
				layer.RaiseOnRemoved();
			}
			layersToDraw_.Remove(layer);
			layersToUpdate_.Remove(layer);
			CoreInstance.RemoveLayer(layer.CoreLayer);
		}

		internal void ImmediatelyRemoveComponent(string key)
		{
			ComponentManager.ImmediatelyRemoveComponent(key);
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="component"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Register(SceneComponent component)
		{
			component.Owner = this;
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="component"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __Unregister(SceneComponent component)
		{
			component.Owner = null;
		}

		/// <summary>
		/// 内部用のメソッドで、ユーザーは呼び出してはいけない。
		/// </summary>
		/// <param name="disposeNative"></param>
		[EditorBrowsable(EditorBrowsableState.Never)]
		public void __DisposeImmediately(bool disposeNative)
		{
			if(IsAlive)
			{
				IsAlive = false;
				OnDispose();
				foreach (var component in ComponentManager.Components)
				{
					component.RaiseOnDisposed();
				}
				foreach(var layer in layersToUpdate_)
				{
					layer.Dispose(disposeNative);
				}
				if(disposeNative)
				{
					ForceToRelease();
				}
			}
		}
		#endregion

		#region イベントハンドラ
		/// <summary>
		/// オーバーライドして、このシーンがエンジンに登録されたときに実行する処理を記述できる。
		/// </summary>
		protected virtual void OnRegistered()
		{
		}

		/// <summary>
		/// オーバーライドして、シーンの更新が始まったときに実行する処理を記述できる。
		/// </summary>
		protected virtual void OnStartUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、トランジション終了時に実行する処理を記述できる。
		/// </summary>
		protected virtual void OnTransitionFinished()
		{
		}

		/// <summary>
		/// オーバーライドして、このシーンから他のシーンへ変わり始めるときに実行する処理を記述できる。
		/// </summary>
		protected virtual void OnTransitionBegin()
		{
		}

		/// <summary>
		/// オーバーライドして、このシーンの更新が止まるときに実行する処理を記述できる。
		/// </summary>
		protected virtual void OnStopUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このシーンがエンジンから登録解除されたときに実行する処理を記述できる。
		/// </summary>
		protected virtual void OnUnregistered()
		{
		}

		/// <summary>
		/// オーバーライドして、このシーンが破棄される際に実行される処理を記述する。
		/// </summary>
		protected virtual void OnDispose()
		{
		}

		/// <summary>
		/// オーバーライドして、Updateの直前に実行する処理を記述する。
		/// </summary>
		protected virtual void OnUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、Updateの直後に実行する処理を記述する。
		/// </summary>
		protected virtual void OnUpdated()
		{
		}

		internal void RaiseOnRegistered()
		{
			OnRegistered();
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnRegistered();
			}
		}

		internal void RaiseOnStartUpdating()
		{
			OnStartUpdating();
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnStartUpdating();
			}
		}

		internal void RaiseOnTransitionFinished()
		{
			OnTransitionFinished();
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnTransitionFinished();
			}
		}

		internal void RaiseOnTransitionBegin()
		{
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnTransitionBegin();
			}
			OnTransitionBegin();
		}

		internal void RaiseOnStopUpdating()
		{
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnStopUpdating();
			}
			OnStopUpdating();
		}

		internal void RaiseOnUnregistered()
		{
			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnUnregistered();
			}
			OnUnregistered();
		}

		internal void Update()
		{
			if(!IsAlive)
			{
				return;
			}

			asd.Particular.Lambda.SortByUpdatePriority(layersToUpdate_);

			OnUpdating();

			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnUpdating();
			}

			foreach(var item in layersToUpdate_)
			{
				item.BeginUpdating();
			}

			foreach(var item in layersToUpdate_)
			{
				item.Update();
			}

			foreach(var item in layersToUpdate_)
			{
				item.EndUpdating();
			}

			foreach(var component in ComponentManager.Components)
			{
				component.RaiseOnUpdated();
			}

			OnUpdated();
		}

		internal void Draw()
		{
			if(!IsAlive)
			{
				return;
			}

			asd.Particular.Lambda.SortByDrawingPriority(layersToDraw_);

			foreach(var item in layersToDraw_)
			{
				item.DrawAdditionally();
			}

			CoreInstance.BeginDrawing();

			foreach(var item in layersToDraw_)
			{
				item.BeginDrawing();
			}

			foreach(var item in layersToDraw_)
			{
				item.Draw();
			}

			foreach(var item in layersToDraw_)
			{
				item.EndDrawing();
			}

			CoreInstance.EndDrawing();
		}
		#endregion


		internal unsafe swig.CoreScene CoreInstance { get; private set; }

		private ComponentManager<SceneComponent> ComponentManager { get; set; }
		private List<Layer> layersToDraw_;
		private List<Layer> layersToUpdate_;
	}
}
