using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    /// <summary>
    /// マップオブジェクト2Dで描画するテクスチャの情報を扱うチップクラス。
    /// </summary>
    public class Chip2D : IDestroy
    {
        internal swig.CoreChip2D SwigObject { get; set; }

        public Chip2D()
        {
            SwigObject = Engine.ObjectSystemFactory.CreateChip2D();

            var p = SwigObject.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                Particular.Helper.ThrowException("");
            }
            GC.Chip2Ds.AddObject(p, this);
        }

        ~Chip2D()
        {
            Destroy();
        }

        public bool IsDestroyed
        {
            get
            {
                return SwigObject == null;
            }
        }

        public void Destroy()
        {
            lock (this)
            {
                if (SwigObject == null) return;
                GC.Collector.AddObject(SwigObject);
                SwigObject = null;
            }
            Particular.GC.SuppressFinalize(this);
        }

        /// <summary>
        /// 描画するテクスチャを取得または設定する。
        /// </summary>
        public Texture2D Texture
        {
            get { return GC.GenerateTexture2D(swig.Accessor.Chip2D_GetTexture(SwigObject) , GC.GenerationType.Get); }
            set { SwigObject.SetTexture(IG.GetTexture2D(value)); }
        }

        /// <summary>
        /// このチップのテクスチャ上の描画範囲を取得または設定する。
        /// </summary>
        public RectF Src
        {
            get { return SwigObject.GetSrc(); }
            set { SwigObject.SetSrc(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な位置を取得または設定する。
        /// </summary>
        public Vector2DF Position
        {
            get { return SwigObject.GetPosition(); }
            set { SwigObject.SetPosition(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な回転角度を取得または設定する。
        /// </summary>
        public float Angle
        {
            get { return SwigObject.GetAngle(); }
            set { SwigObject.SetAngle(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な拡大率を取得または設定する。
        /// </summary>
        public Vector2DF Scale
        {
            get { return SwigObject.GetScale(); }
            set { SwigObject.SetScale(value); }
        }

        /// <summary>
        /// このチップを描画する際の中心座標を取得または設定する。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get { return SwigObject.GetCenterPosition(); }
            set { SwigObject.SetCenterPosition(value); }
        }

        /// <summary>
        /// このチップのテクスチャを描画する際に合成する色を取得または設定する。
        /// </summary>
        public Color Color
        {
            get { return SwigObject.GetColor(); }
            set { SwigObject.SetColor(value); }
        }

        /// <summary>
        /// このチップのテクスチャ描画の左右反転フラグを取得または設定する。
        /// </summary>
        public bool TurnLR
        {
            get { return SwigObject.GetTurnLR(); }
            set { SwigObject.SetTurnLR(value); }
        }

        /// <summary>
        /// このチップのテクスチャ描画の上下反転フラグを取得または設定する。
        /// </summary>
        public bool TurnUL
        {
            get { return SwigObject.GetTurnUL(); }
            set { SwigObject.SetTurnUL(value); }
        }

        /// <summary>
        /// このチップのブレンディング モードを取得または設定する。
        /// </summary>
        public AlphaBlendMode AlphaBlendMode
        {
            get { return (AlphaBlendMode)SwigObject.GetAlphaBlendMode(); }
            set { SwigObject.SetAlphaBlendMode((swig.AlphaBlendMode)value); }
        }

        /// <summary>
        /// このチップのテクスチャを描画する際のテクスチャフィルタの種類を取得または設定する。
        /// </summary>
        public TextureFilterType TextureFilterType
        {
            get { return (TextureFilterType)SwigObject.GetTextureFilterType(); }
            set { SwigObject.SetTextureFilterType((swig.TextureFilterType)value); }
        }

        /// <summary>
        /// このチップの描画優先度を取得または設定する。
        /// </summary>
        public int DrawingPriority
        {
            get { return SwigObject.GetDrawingPriority(); }
            set { SwigObject.SetDrawingPriority(value); }
        }
    }
}
