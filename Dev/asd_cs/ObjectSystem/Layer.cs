using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace asd
{
	/// <summary>
	/// オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス
	/// </summary>
	public abstract class Layer : IReleasable, IDisposable, IBeingAbleToDisposeNative
	{
		public abstract bool IsReleased { get; }

		/// <summary>
		/// コンストラクタ
		/// </summary>
		public Layer()
		{
			IsAlive = true;
			IsUpdated = true;
			postEffects = new List<PostEffect>();
			Name = "Layer";
			UpdatePriority = 0;
			UpdateFrequency = 1;
			updateTimer = 0;
		}

		#region パラメータ
		/// <summary>
		/// このレイヤーが有効化どうかを取得する。<see cref="Dispose"/>メソッドを呼び出した後なら false。
		/// </summary>
		public bool IsAlive { get; internal set; }

		/// <summary>
		/// レイヤーの更新を実行するかどうか取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// レイヤーを描画するかどうか取得または設定する。
		/// </summary>
		public bool IsDrawn
		{
			get { return CoreLayer.GetIsDrawn(); }
			set { CoreLayer.SetIsDrawn(value); }
		}

		/// <summary>
		/// このレイヤーの更新の優先順位を取得または設定する。
		/// </summary>
		public int UpdatePriority { get; set; }

		/// <summary>
		/// このレイヤーの１フレームごとの更新回数を取得または設定する。
		/// </summary>
		/// <returns></returns>
		public float UpdateFrequency { get; set; }

		/// <summary>
		/// このインスタンスを管理している asd.Scene クラスのインスタンスを取得する。
		/// </summary>
		public Scene Scene { get; internal set; }

		/// <summary>
		/// このレイヤーの前回の更新時間を取得する。
		/// </summary>
		public int TimeForUpdate
		{
			get { return CoreLayer.GetTimeForUpdate(); }
		}

		/// <summary>
		/// このレイヤーに登録されているオブジェクトの数を取得する。
		/// </summary>
		public abstract int ObjectCount { get; }

		/// <summary>
		/// このレイヤーの名前を取得または設定する。
		/// </summary>
		public string Name { get; set; }

		/// <summary>
		/// このレイヤーの描画優先度を取得または設定する。この値が大きいほど手前に描画される。
		/// </summary>
		public int DrawingPriority
		{
			get { return CoreLayer.GetDrawingPriority(); }
			set { CoreLayer.SetDrawingPriority(value); }
		}

		/// <summary>
		/// レイヤーの種類を取得する。
		/// </summary>
		public abstract LayerType LayerType { get; }
		#endregion


		#region イベント
		internal virtual void RaiseOnAdded()
		{
			OnAdded();
		}

		internal virtual void RaiseOnRemoved()
		{
			OnRemoved();
		}

		/// <summary>
		/// このレイヤーを破棄する。
		/// </summary>
		public virtual void Dispose()
		{
			Dispose(false);
		}

		/// <summary>
		/// このレイヤーを破棄する。
		/// </summary>
		/// <param name="disposeNative">ネイティブ リソースも即解放するかどうかの真偽値。</param>
		/// <remarks>登録されているオブジェクトもすべて破棄するが、オブジェクトの破棄はこのメソッドを呼び出したフレームの最後に実行されるので注意が必要。</remarks>
		public void Dispose(bool disposeNative)
		{
			Engine.ChangesToBeCommited.Enqueue(new EventToDisposeContent(this, disposeNative));
		}

		void IBeingAbleToDisposeNative.DisposeImmediately(bool disposeNative)
		{
			if (IsAlive)
			{
				IsAlive = false;
				OnDispose();
				DisposeContents(disposeNative);
				if(Scene != null)
				{
					Scene.ImmediatelyRemoveLayer(this, false);
				}
				if(disposeNative)
				{
					ForceToRelease();
				}
			}
		}

		internal abstract void DisposeContents(bool disposeNative);

		internal abstract void BeginUpdating();

		internal abstract void EndUpdating();

		internal virtual void Update()
		{
			if(!IsAlive || !isUpdatedCurrent)
			{
				return;
			}

			CoreLayer.BeginMeasureUpdateTime();

			updateTimer += UpdateFrequency;
			while(updateTimer >= 1)
			{
				OnUpdating();
				UpdateInternal();
				OnUpdated();
				updateTimer -= 1;
			}

			CoreLayer.EndMeasureUpdateTime();
		}

		internal abstract void UpdateInternal();

		internal void BeginDrawing()
		{
			if(!IsAlive)
			{
				return;
			}

			Scene.CoreInstance.SetRenderTargetForDrawingLayer();
			CoreLayer.BeginDrawing();
		}

		internal void EndDrawing()
		{
			if(!IsAlive)
			{
				return;
			}

			CoreLayer.EndDrawing();

			if(postEffects.Count > 0)
			{
				foreach(var p in postEffects)
				{
					Scene.CoreInstance.BeginPostEffect(p.CoreInstance);

					var src_ = Scene.CoreInstance.GetSrcTarget();
					var dst_ = Scene.CoreInstance.GetDstTarget();

					RenderTexture2D src = GC.GenerateRenderTexture2D(src_, GenerationType.Get);
					RenderTexture2D dst = GC.GenerateRenderTexture2D(dst_, GenerationType.Get);

					p.Draw(dst, src);

					Scene.CoreInstance.EndPostEffect(p.CoreInstance);
				}
			}
		}

		internal void Draw()
		{
			if(IsAlive)
			{
				CoreLayer.Draw();
			}
		}

		internal abstract void DrawAdditionally();

		/// <summary>
		/// オーバーライドして、このレイヤーがシーンに登録されたときの処理を記述できる。
		/// </summary>
		protected virtual void OnAdded()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーがシーンから登録解除されたときの処理を記述できる。
		/// </summary>
		protected virtual void OnRemoved()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーが破棄されるときの処理を記述できる。
		/// </summary>
		protected virtual void OnDispose()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーが更新される前の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdating()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーが更新された後の処理を記述できる。
		/// </summary>
		protected virtual void OnUpdated()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーの追加の描画処理を記述できる。
		/// </summary>
		protected virtual void OnDrawAdditionally()
		{
		}
		#endregion


		/// <summary>
		/// ポストエフェクトを追加する。
		/// </summary>
		/// <param name="postEffect">ポストエフェクト</param>
		public void AddPostEffect(PostEffect postEffect)
		{
			ThrowIfReleased();
			CoreLayer.Draw();
			postEffects.Add(postEffect);
			CoreLayer.AddPostEffect(postEffect.CoreInstance);
		}

		/// <summary>
		/// ポストエフェクトを全て消去する。
		/// </summary>
		public void ClearPostEffects()
		{
			ThrowIfReleased();
			CoreLayer.Draw();
			postEffects.Clear();
			CoreLayer.ClearPostEffects();
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public abstract void ForceToRelease();

		internal void ThrowIfDisposed()
		{
			if(!IsAlive)
			{
				throw new ObjectDisposedException(GetType().FullName);
			}
		}

		internal void ThrowIfReleased()
		{
			if(IsReleased)
			{
				throw new ObjectDisposedException(GetType().FullName);
			}
		}
		

		private float updateTimer;

		private swig.CoreLayer coreLayer_;

		internal swig.CoreLayer CoreLayer
		{
			get { return coreLayer_; }
			set { coreLayer_ = value; }
		}

		internal List<PostEffect> postEffects;

		internal bool isUpdatedCurrent;
	}
}
