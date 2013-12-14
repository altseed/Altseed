using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ace
{
	/// <summary>
	/// オブジェクトの更新と描画を管理するレイヤーの機能を提供する抽象クラス
	/// </summary>
	public abstract class Layer
	{
		internal swig.CoreLayer commonObject = null;

		public Layer()
		{
			IsUpdated = true;
			postEffects = new List<PostEffect>();
		}

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
		/// このインスタンスを管理している ace.Scene クラスのインスタンスを取得する。
		/// </summary>
		public Scene Scene { get; internal set; }

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

		internal abstract void DrawAdditionally();

		internal void BeginDrawing()
		{
			commonObject.BeginDrawing();
		}

		internal void EndDrawing()
		{
			commonObject.EndDrawing();

			if (postEffects.Count > 0)
			{
				var rtf_ = commonObject.GetFirstRenderTarget();
				var rt0_ = commonObject.GetRenderTarget0();
				var rt1_ = commonObject.GetRenderTarget1();

				RenderTexture2D rtf = GC.GenerateRenderTexture2D(rtf_);
				RenderTexture2D rt0 = GC.GenerateRenderTexture2D(rt0_);
				RenderTexture2D rt1 = GC.GenerateRenderTexture2D(rt1_);

				int index = 0;
				foreach (var p in postEffects)
				{
					if (index == 0)
					{
						p.OnDraw(rt1, rtf);
					}
					else if (index % 2 == 0)
					{
						p.OnDraw(rt1, rt0);
					}
					else
					{
						p.OnDraw(rt0, rt1);
					}
					index++;
				}

				commonObject.SetTargetToLayer(index % 2);
			}

			commonObject.EndDrawingAfterEffects();
		}


		internal swig.CoreLayer CoreLayer { get { return commonObject; } }

		protected virtual void OnUpdating()
		{
		}

		protected virtual void OnUpdated()
		{
		}

		protected virtual void OnDrawAdditionally()
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
		public void ClearPostEffect()
		{
			postEffects.Clear();
			commonObject.ClearPostEffects();
		}

		protected List<PostEffect> postEffects;

	}
}
