using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// 2Dオブジェクトの更新と描画を管理するレイヤーの機能を提供するクラス。
	/// </summary>
	public class Layer2D : Layer, IDestroy
	{
		/// <summary>
		/// ace.Layer2D クラスの新しいインスタンスを初期化します。
		/// </summary>
		public Layer2D()
		{
			coreLayer2D = Engine.ObjectSystemFactory.CreateLayer2D();

			var p = coreLayer2D.GetPtr();
			if( GC.Layer2Ds.GetObject(p) != null)
			{
				throw new Exception();
			}

			GC.Layer2Ds.AddObject(p, this);

			objects_ = new List<Object2DBase>();
			postEffects = new List<PostEffect>();
		}

		#region GC対策
		~Layer2D()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return coreLayer2D == null; }
		}

		public void Destroy()
		{
			lock( this )
			{
				if( coreLayer2D == null ) return;
				GC.Collector.AddObject( coreLayer2D );
				coreLayer2D = null;
			}
			System.GC.SuppressFinalize( this );
		} 
		#endregion


		/// <summary>
		/// このインスタンスを管理している ace.Scene クラスのインスタンスを取得します。
		/// </summary>
		public override Scene Scene { get; internal set; }

		/// <summary>
		/// このレイヤーが管理している2Dオブジェクトのコレクションを取得します。
		/// </summary>
		public IEnumerable<Object2DBase> Objects
		{
			get { return objects_; }
		}

		List<PostEffect> postEffects;

		/// <summary>
		/// このレイヤーの描画優先度を取得または設定します。この値が大きいほど手前に描画されます。
		/// </summary>
		public override int DrawingPriority
		{
			get { return coreLayer2D.GetDrawingPriority(); }
			set { coreLayer2D.SetDrawingPriority( value ); }
		}

		/// <summary>
		/// このレイヤーに指定した2Dオブジェクトを追加します。
		/// </summary>
		/// <param name="object2D">追加する2Dオブジェクト。</param>
		public void AddObject(Object2DBase object2D)
		{
			if( object2D.Layer != null )
			{
				throw new InvalidOperationException( "指定したオブジェクトは既に別のレイヤーに所属しています。" );
			}
			objects_.Add(object2D);
			coreLayer2D.AddObject(object2D.CoreObject);
			object2D.Layer = this;
			object2D.Start();
		}

		/// <summary>
		/// このレイヤーから指定した2Dオブジェクトを削除します。
		/// </summary>
		/// <param name="object2D">削除される2Dオブジェクト。</param>
		public void RemoveObject(Object2DBase object2D)
		{
			objects_.Remove(object2D);
			coreLayer2D.RemoveObject(object2D.CoreObject);
			object2D.Layer = null;
		}

		/// <summary>
		/// ポストエフェクトを追加する。
		/// </summary>
		/// <param name="postEffect">ポストエフェクト</param>
		public void AddPostEffect(PostEffect postEffect)
		{
			postEffects.Add(postEffect);
			coreLayer2D.AddPostEffect(postEffect.SwigObject);
		}

		/// <summary>
		/// ポストエフェクトを全て消去する。
		/// </summary>
		public void ClearPostEffect()
		{
			postEffects.Clear();
			coreLayer2D.ClearPostEffects();
		}

		protected virtual void OnUpdating()
		{
		}

		protected virtual void OnUpdated()
		{
		}

		protected virtual void OnDrawAdditionally()
		{
		}

		internal override swig.CoreLayer CoreLayer
		{
			get { return coreLayer2D; }
		}

		internal override void Update()
		{
			OnUpdating();

			foreach( var item in objects_ )
			{
				item.Update();
			}

			OnUpdated();
		}

		internal override void DrawAdditionally()
		{
			OnDrawAdditionally();
			foreach( var item in objects_ )
			{
				item.DrawAdditionally();
			}
		}

		internal override void BeginDrawing()
		{
			coreLayer2D.BeginDrawing();
		}

		internal override void EndDrawing()
		{
			coreLayer2D.EndDrawing();

			if (postEffects.Count > 0)
			{
				var rt0_ = coreLayer2D.GetRenderTarget0();
				var rt1_ = coreLayer2D.GetRenderTarget1();
				RenderTexture2D rt0 = null;
				RenderTexture2D rt1 = null;

				var existingSrc = GC.Texture2Ds.GetObject(rt0_.GetPtr());
				if (existingSrc != null)
				{
					rt0 = (RenderTexture2D)existingSrc;
				}

				var existingDst = GC.Texture2Ds.GetObject(rt1_.GetPtr());
				if (existingDst != null)
				{
					rt1 = (RenderTexture2D)existingDst;
				}

				if (rt0 == null)
				{
					rt0_.AddRef();
					rt0 = new RenderTexture2D(rt0_);
					GC.Texture2Ds.AddObject(rt0_.GetPtr(), rt0);
				}

				if (rt1 == null)
				{
					rt1_.AddRef();
					rt1 = new RenderTexture2D(rt1_);
					GC.Texture2Ds.AddObject(rt1_.GetPtr(), rt1);
				}

				int index = 0;
				foreach (var p in postEffects)
				{
					if (index % 2 == 0)
					{
						p.OnDraw(rt1, rt0);
					}
					else
					{
						p.OnDraw(rt0, rt1);
					}
					index++;
				}

				coreLayer2D.SetTargetToLayer(index % 2);
			}

			coreLayer2D.EndDrawingAfterEffects();
		}

		private swig.CoreLayer2D coreLayer2D { get; set; }

		private List<Object2DBase> objects_ { get; set; }
	}
}
