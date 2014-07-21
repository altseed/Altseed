using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// ポストエフェクトを適用するクラス
	/// </summary>
	public class PostEffect : IDestroy
	{
		internal swig.CorePostEffect SwigObject;

		public PostEffect()
		{
			SwigObject = Engine.ObjectSystemFactory.CreatePostEffect();

			var p = SwigObject.GetPtr();
			if (GC.PostEffects.GetObject(p) != null)
			{
				throw new Exception();
			}
			GC.PostEffects.AddObject(p, this);
		}

		#region GC対応
		~PostEffect()
		{
			Destroy();
		}

		public bool IsDestroyed
		{
			get { return SwigObject == null; }
		}

		public void Destroy()
		{
			lock (this)
			{
				if (SwigObject == null) return;
				GC.Collector.AddObject(SwigObject);
				SwigObject = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion

		/**
		@brief	
		*/
		/// <summary>
		/// オーバーライドして、毎フレーム描画される処理を記述できる。
		/// </summary>
		public virtual void OnDraw(RenderTexture2D dst, RenderTexture2D src) { }

		/// <summary>
		/// マテリアルを用いてテクスチャに画像を描画する。
		/// </summary>
		/// <param name="target">描画先</param>
		/// <param name="material">マテリアル</param>
		public void DrawOnTexture2DWithMaterial(RenderTexture2D target, Material2D material)
		{
			SwigObject.DrawOnTexture2DWithMaterial(IG.GetRenderTexture2D(target), IG.GetMaterial2D(material));
		}
	}
}
