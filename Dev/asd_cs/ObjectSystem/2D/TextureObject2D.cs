using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.swig;

namespace asd
{
	/// <summary>
	/// テクスチャを描画するクラス。
	/// </summary>
	public partial class TextureObject2D : DrawnObject2D, IReleasable
	{
		/// <summary>
		/// asd.TextureObject2D の新しいインスタンスを初期化する。
		/// </summary>
		public TextureObject2D()
		{
			renderedObject = Engine.ObjectSystemFactory.CreateTextureObject2D();

			var p = renderedObject.GetPtr();
			if(GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}
			GC.Object2Ds.AddObject(p, this);
		}

		#region GC対応
		~TextureObject2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return renderedObject == null; }
		}

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
		public override void ForceToRelease()
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
		/// この2Dオブジェクトの描画に使用するテクスチャを取得または設定する。
		/// </summary>
		public Texture2D Texture
		{
			get
			{
				ThrowIfDisposed();
				return GC.GenerateTexture2D(renderedObject.GetTexture(), GC.GenerationType.Get);
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetTexture(IG.GetTexture2D(value));
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画原点を取得または設定する。描画原点は拡大・回転・描画の中心となる、画像データ内での座標。
		/// </summary>
		public Vector2DF CenterPosition
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetCenterPosition();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetCenterPosition(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に合成する色を取得または設定する。
		/// </summary>
		public override Color Color
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetColor();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetColor(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に左右反転するかどうかの真偽値を取得または設定する。
		/// </summary>
		public bool TurnLR
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetTurnLR();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetTurnLR(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に上下反転するかどうかの真偽値を取得または設定する。
		/// </summary>
		public bool TurnUL
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetTurnUL();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetTurnUL(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
		/// </summary>
		public override int DrawingPriority
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetDrawingPriority();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetDrawingPriority(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際のブレンドモードを取得または設定する。
		/// </summary>
		public AlphaBlendMode AlphaBlend
		{
			get
			{
				ThrowIfDisposed();
				return (AlphaBlendMode)renderedObject.GetAlphaBlendMode();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetAlphaBlendMode((swig.AlphaBlendMode)value);
			}
		}

		///<summary>
		/// テクスチャ上の描画範囲を設定する。
		/// </summary>
		/// <remarks>負の値は無効であり、負の値が含まれる場合はテクスチャ全体を描画する。</remarks>
		public RectF Src
		{
			get
			{
				ThrowIfDisposed();
				return renderedObject.GetSrc();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetSrc(value);
			}
		}

		///<summary>
		/// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
		/// </summary>
		public TextureFilterType TextureFilterType
		{
			get
			{
				ThrowIfDisposed();
				return (TextureFilterType)renderedObject.GetTextureFilterType();
			}
			set
			{
				ThrowIfDisposed();
				renderedObject.SetTextureFilterType((swig.TextureFilterType)value);
			}
		}

		internal override swig.CoreObject2D CoreObject
		{
			get { return renderedObject; }
		}

		internal override CoreDrawnObject2D CoreDrawnObject
		{
			get { return renderedObject; }
		}

		private swig.CoreTextureObject2D renderedObject { get; set; }
	}
}
