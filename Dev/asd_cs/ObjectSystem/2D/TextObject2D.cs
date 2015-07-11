using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 文字列の描画を扱うクラス。
    /// </summary>
    public partial class TextObject2D : Object2D, IDestroy
	{

		internal override swig.CoreObject2D CoreObject
		{
			get { return coreTextObject; }
		}
		private swig.CoreTextObject2D coreTextObject { get; set; }

		public TextObject2D()
		{
			coreTextObject = Engine.ObjectSystemFactory.CreateTextObject2D();

			var p = coreTextObject.GetPtr();
			if (GC.Object2Ds.GetObject(p) != null)
			{
				Particular.Helper.ThrowException("");
			}

			GC.Object2Ds.AddObject(p, this);
		}
		
#region GC対策
		~TextObject2D()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return coreTextObject == null; }
		}

		public override void Destroy()
		{
			lock( this )
			{
				if( coreTextObject == null ) return;
				GC.Collector.AddObject( coreTextObject );
				coreTextObject = null;
			}
			Particular.GC.SuppressFinalize( this );
		}
        #endregion



        /// <summary>
        /// この2Dオブジェクトの描画に使用するフォントを取得または設定する。
        /// </summary>
        public Font Font
        {
            get { return GC.GenerateFont(coreTextObject.GetFont(), GC.GenerationType.Get); }
            set { coreTextObject.SetFont(IG.GetFont(value)); }
        }

        /// <summary>
        /// 描画する文字列を取得または設定する。
        /// </summary>
        /// <returns></returns>
        public string Text
        {
            get { return coreTextObject.GetText(); }
            set { coreTextObject.SetText(value); }
        }

        /// <summary>
        /// 文字列の描画方向を指定する。
        /// </summary>
        /// <returns></returns>
        public WritingDirection WritingDirection
        {
            get { return (WritingDirection)coreTextObject.GetWritingDirection(); }
            set { coreTextObject.SetWritingDirection((swig.WritingDirection)value); }
        }

        /// <summary>
        /// テキストを描画する際の文字間値を取得または設定する。
        /// </summary>
        /// <returns></returns>
        public float LetterSpacing
        {
            get { return coreTextObject.GetLetterSpacing(); }
            set { coreTextObject.SetLetterSpacing(value); }
        }

        /// <summary>
        /// テキストを描画する際の行間値を取得または設定する。
        /// </summary>
        /// <returns></returns>
        public float LineSpacing
        {
            get { return coreTextObject.GetLineSpacing(); }
            set { coreTextObject.SetLineSpacing(value); }
        }

        /// <summary>
        /// この2Dオブジェクトを描画する際の描画原点を取得または設定する。描画原点は拡大・回転・描画の中心となる、画像データ内での座標。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get { return coreTextObject.GetCenterPosition(); }
            set { coreTextObject.SetCenterPosition(value); }
        }
        /// <summary>
        /// この2Dオブジェクトを描画する際に合成する色を取得または設定する。
        /// </summary>
        public Color Color
        {
            get { return coreTextObject.GetColor(); }
            set { coreTextObject.SetColor(value); }
        }
        /// <summary>
        /// この2Dオブジェクトを描画する際に左右反転するかどうかの真偽値を取得または設定する。
        /// </summary>
        public bool TurnLR
        {
            get { return coreTextObject.GetTurnLR(); }
            set { coreTextObject.SetTurnLR(value); }
        }
        /// <summary>
        /// この2Dオブジェクトを描画する際に上下反転するかどうかの真偽値を取得または設定する。
        /// </summary>
        public bool TurnUL
        {
            get { return coreTextObject.GetTurnUL(); }
            set { coreTextObject.SetTurnUL(value); }
        }
        /// <summary>
        /// この2Dオブジェクトを描画する際の描画優先度を取得または設定する。描画優先度が高いほど手前に描画される。
        /// </summary>
        public int DrawingPriority
        {
            get { return coreTextObject.GetDrawingPriority(); }
            set { coreTextObject.SetDrawingPriority(value); }
        }
        /// <summary>
        /// この2Dオブジェクトを描画する際のブレンドモードを取得または設定する。
        /// </summary>
        public AlphaBlendMode AlphaBlend
        {
            get { return (AlphaBlendMode)coreTextObject.GetAlphaBlendMode(); }
            set { coreTextObject.SetAlphaBlendMode((swig.AlphaBlendMode)value); }
        }

        ///<summary>
        /// この2Dオブジェクトを描画する際のテクスチャフィルタを取得または設定する。
        /// </summary>
        public TextureFilterType TextureFilterType
        {
            get { return (TextureFilterType)coreTextObject.GetTextureFilterType();}
            set { coreTextObject.SetTextureFilterType((swig.TextureFilterType)value); }
        }
    }
}
