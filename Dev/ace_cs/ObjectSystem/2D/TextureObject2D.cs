using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	/// <summary>
	/// テクスチャを描画するクラス。
	/// </summary>
	public partial class TextureObject2D : Object2D, IDestroy
	{
		/// <summary>
		/// ace.TextureObject2D の新しいインスタンスを初期化します。
		/// </summary>
		public TextureObject2D()
		{
			renderedObject = Engine.ObjectSystemFactory.CreateTextureObject2D();

			var p = renderedObject.GetPtr();
			if (GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}
			GC.Object2Ds.AddObject(p, this);
		}

		#region GC対応
		~TextureObject2D()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return renderedObject == null; }
		}

		public override void Destroy()
		{
			lock (this)
			{
				if(renderedObject == null) return;
				GC.Collector.AddObject(renderedObject);
				renderedObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion


		/// <summary>
		/// この2Dオブジェクトの描画に使用するテクスチャを取得または設定します。
		/// </summary>
		public Texture2D Texture
		{
			get { return GC.GenerateTexture2D(renderedObject.GetTexture(), GC.GenerationType.Get); }
			set { renderedObject.SetTexture(IG.GetTexture2D(value)); }
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画原点を取得または設定します。描画原点は拡大・回転・描画の中心となる、画像データ内での座標です。
		/// </summary>
		public Vector2DF CenterPosition
		{
			get { return renderedObject.GetCenterPosition(); }
			set { renderedObject.SetCenterPosition(value); }
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に合成する色を取得または設定します。
		/// </summary>
		public Color Color
		{
			get { return renderedObject.GetColor(); }
			set { renderedObject.SetColor(value); }
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に左右反転するかどうかの真偽値を取得または設定します。
		/// </summary>
		public bool TurnLR
		{
			get { return renderedObject.GetTurnLR(); }
			set { renderedObject.SetTurnLR(value); }
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に上下反転するかどうかの真偽値を取得または設定します。
		/// </summary>
		public bool TurnUL
		{
			get { return renderedObject.GetTurnUL(); }
			set { renderedObject.SetTurnUL(value); }
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定します。描画優先度が高いほど手前に描画されます。
		/// </summary>
		public int DrawingPriority
		{
			get { return renderedObject.GetDrawingPriority(); }
			set { renderedObject.SetDrawingPriority(value); }
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際のブレンドモードを取得または設定します。
		/// </summary>
		public AlphaBlendMode AlphaBlend
		{
			get { return (AlphaBlendMode)renderedObject.GetAlphaBlendMode(); }
			set { renderedObject.SetAlphaBlendMode((swig.AlphaBlendMode)value); }
		}

		///<summary>
		/// テクスチャ上の描画範囲を設定する。(フィールドのいずれかが0より小さかったらテクスチャ全体を描画する。)
		/// </summary>
		public RectF Src
		{
			get { return renderedObject.GetSrc(); }
			set { renderedObject.SetSrc(value); }
		}

		///<summary>
		/// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
		/// </summary>
		public TextureFilterType TextureFilterType
		{
			get { return (TextureFilterType)renderedObject.GetTextureFilterType(); }
			set { renderedObject.SetTextureFilterType((swig.TextureFilterType)value); }
		}

		internal override swig.CoreObject2D CoreObject
		{
			get { return renderedObject; }
		}

		private swig.CoreTextureObject2D renderedObject { get; set; }
	}
}
