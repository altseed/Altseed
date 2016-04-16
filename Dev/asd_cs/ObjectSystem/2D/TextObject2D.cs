using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using asd.swig;

namespace asd
{
	/// <summary>
	/// 文字列の描画を扱うクラス。
	/// </summary>
	public partial class TextObject2D : DrawnObject2D, IReleasable
	{
		internal override swig.CoreObject2D CoreObject
		{
			get { return coreTextObject; }
		}
		internal override CoreDrawnObject2D CoreDrawnObject
		{
			get { return coreTextObject; }
		}
		private swig.CoreTextObject2D coreTextObject { get; set; }

		public TextObject2D()
		{
			coreTextObject = Engine.ObjectSystemFactory.CreateTextObject2D();

			var p = coreTextObject.GetPtr();
			if(GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}

			GC.Object2Ds.AddObject(p, this);
		}

		#region GC対策
		~TextObject2D()
		{
			ForceToRelease();
		}

		public override bool IsReleased
		{
			get { return coreTextObject == null; }
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
				if(coreTextObject == null) return;
				GC.Collector.AddObject(coreTextObject);
				coreTextObject = null;
			}
			Particular.GC.SuppressFinalize(this);
		}
		#endregion



		/// <summary>
		/// この2Dオブジェクトの描画に使用するフォントを取得または設定する。
		/// </summary>
		public Font Font
		{
			get
			{
				ThrowIfReleased();
				return GC.GenerateFont(coreTextObject.GetFont(), GenerationType.Get);
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetFont(IG.GetFont(value));
			}
		}

		/// <summary>
		/// 描画する文字列を取得または設定する。
		/// </summary>
		/// <returns></returns>
		public string Text
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetText();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetText(value);
			}
		}

		/// <summary>
		/// 文字列の描画方向を指定する。
		/// </summary>
		/// <returns></returns>
		public WritingDirection WritingDirection
		{
			get
			{
				ThrowIfReleased();
				return (WritingDirection)coreTextObject.GetWritingDirection();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetWritingDirection((swig.WritingDirection)value);
			}
		}

		/// <summary>
		/// テキストを描画する際の文字間値を取得または設定する。
		/// </summary>
		/// <returns></returns>
		public float LetterSpacing
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetLetterSpacing();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetLetterSpacing(value);
			}
		}

		/// <summary>
		/// テキストを描画する際の行間値を取得または設定する。
		/// </summary>
		/// <returns></returns>
		public float LineSpacing
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetLineSpacing();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetLineSpacing(value);
			}
		}

		/// <summary>
		/// この2Dオブジェクトを描画する際の描画原点を取得または設定する。描画原点は拡大・回転・描画の中心となる、画像データ内での座標。
		/// </summary>
		public Vector2DF CenterPosition
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetCenterPosition();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetCenterPosition(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に合成する色を取得または設定する。
		/// </summary>
		public override Color Color
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetColor();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetColor(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に左右反転するかどうかの真偽値を取得または設定する。
		/// </summary>
		public bool TurnLR
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetTurnLR();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetTurnLR(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際に上下反転するかどうかの真偽値を取得または設定する。
		/// </summary>
		public bool TurnUL
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetTurnUL();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetTurnUL(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
		/// </summary>
		public override int DrawingPriority
		{
			get
			{
				ThrowIfReleased();
				return coreTextObject.GetDrawingPriority();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetDrawingPriority(value);
			}
		}
		/// <summary>
		/// この2Dオブジェクトを描画する際のブレンドモードを取得または設定する。
		/// </summary>
		public AlphaBlendMode AlphaBlend
		{
			get
			{
				ThrowIfReleased();
				return (AlphaBlendMode)coreTextObject.GetAlphaBlendMode();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetAlphaBlendMode((swig.AlphaBlendMode)value);
			}
		}

		///<summary>
		/// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
		/// </summary>
		public TextureFilterType TextureFilterType
		{
			get
			{
				ThrowIfReleased();
				return (TextureFilterType)coreTextObject.GetTextureFilterType();
			}
			set
			{
				ThrowIfReleased();
				coreTextObject.SetTextureFilterType((swig.TextureFilterType)value);
			}
		}
	}
}
