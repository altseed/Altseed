using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace asd
{
	/// <summary>
	/// オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス
	/// </summary>
	public abstract class Layer
	{
		internal swig.CoreLayer commonObject = null;

		public Layer()
		{
			IsAlive = true;
			IsUpdated = true;
			postEffects = new List<PostEffect>();
			Name = "Layer";
		}

		/// <summary>
		/// このレイヤーが有効化どうかを取得する。Vanishメソッドを呼び出した後なら false。
		/// </summary>
		public bool IsAlive { get; private set; }

		/// <summary>
		/// レイヤーの更新を実行するかどうか取得または設定する。
		/// </summary>
		public bool IsUpdated { get; set; }

		/// <summary>
		/// レイヤーを描画するかどうか取得または設定する。
		/// </summary>
		public bool IsDrawn
		{
			get { return commonObject.GetIsDrawn(); }
			set { commonObject.SetIsDrawn(value); }
		}

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
			get { return commonObject.GetDrawingPriority(); }
			set { commonObject.SetDrawingPriority(value); }
		}

		internal abstract void BeginUpdating();
		internal abstract void EndUpdating();

		internal abstract void Update();

		internal abstract void CallDestroy();

		internal abstract void DrawAdditionally();

		internal void BeginDrawing()
		{
			Scene.CoreScene.SetRenderTargetForDrawingLayer();
			commonObject.BeginDrawing();
		}

		internal void EndDrawing()
		{
			commonObject.EndDrawing();

			if(postEffects.Count > 0)
			{
				foreach(var p in postEffects)
				{
					Scene.CoreScene.BeginPostEffect(p.SwigObject);

					var src_ = Scene.CoreScene.GetSrcTarget();
					var dst_ = Scene.CoreScene.GetDstTarget();

					RenderTexture2D src = GC.GenerateRenderTexture2D(src_, GC.GenerationType.Get);
					RenderTexture2D dst = GC.GenerateRenderTexture2D(dst_, GC.GenerationType.Get);

					p.OnDraw(dst, src);

					Scene.CoreScene.EndPostEffect(p.SwigObject);
				}
			}
		}

		internal void Draw()
		{
			commonObject.Draw();
		}

		internal swig.CoreLayer CoreLayer { get { return commonObject; } }

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

		/// <summary>
		/// オーバーライドして、このレイヤーがVansihメソッドによって破棄されるときの処理を記述できる。
		/// </summary>
		protected virtual void OnVanish()
		{
		}

		/// <summary>
		/// オーバーライドして、このレイヤーが破棄されるときの処理を記述できる。
		/// </summary>
		protected virtual void OnDispose()
		{
		}

		/// <summary>
		/// ポストエフェクトを追加する。
		/// </summary>
		/// <param name="postEffect">ポストエフェクト</param>
		public void AddPostEffect(PostEffect postEffect)
		{
			postEffects.Add(postEffect);
			commonObject.AddPostEffect(postEffect.SwigObject);
		}

		/// <summary>
		/// ポストエフェクトを全て消去する。
		/// </summary>
		public void ClearPostEffects()
		{
			postEffects.Clear();
			commonObject.ClearPostEffects();
		}

		/// <summary>
		/// このレイヤーを破棄する。
		/// </summary>
		public void Vanish()
		{
			IsAlive = false;
			OnVanish();
		}

		/// <summary>
		/// レイヤーの種類を取得する。
		/// </summary>
		public abstract LayerType LayerType { get; }

		protected List<PostEffect> postEffects;

		protected bool isUpdatedCurrent;
	}
}
