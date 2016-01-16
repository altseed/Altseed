using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// マップオブジェクト2Dで描画するテクスチャの情報を扱うチップクラス。
    /// </summary>
    public class Chip2D : IReleasable
    {
        internal swig.CoreChip2D CoreInstance { get; set; }

        public Chip2D()
        {
            CoreInstance = Engine.ObjectSystemFactory.CreateChip2D();

            var p = CoreInstance.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Chip2Ds.AddObject(p, this);
        }

        ~Chip2D()
        {
            ForceToRelease();
        }

        public bool IsReleased
        {
            get
            {
                return CoreInstance == null;
            }
        }

        /// <summary>
        /// 強制的に使用しているメモリを開放する。
        /// </summary>
        /// <remarks>
        /// 何らかの理由でメモリが不足した場合に実行する。
        /// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
        /// </remarks>
        public void ForceToRelease()
        {
            lock (this)
            {
                if (CoreInstance == null) return;
                GC.Collector.AddObject(CoreInstance);
                CoreInstance = null;
            }
            Particular.GC.SuppressFinalize(this);
        }

        /// <summary>
        /// 描画するテクスチャを取得または設定する。
        /// </summary>
        public Texture2D Texture
        {
            get { return GC.GenerateTexture2D(swig.Accessor.Chip2D_GetTexture(CoreInstance), GC.GenerationType.Get); }
            set { CoreInstance.SetTexture(IG.GetTexture2D(value)); }
        }

        /// <summary>
        /// このチップのテクスチャ上の描画範囲を取得または設定する。
        /// </summary>
        public RectF Src
        {
            get { return CoreInstance.GetSrc(); }
            set { CoreInstance.SetSrc(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な位置を取得または設定する。
        /// </summary>
        public Vector2DF Position
        {
            get { return CoreInstance.GetPosition(); }
            set { CoreInstance.SetPosition(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な回転角度を取得または設定する。
        /// </summary>
        public float Angle
        {
            get { return CoreInstance.GetAngle(); }
            set { CoreInstance.SetAngle(value); }
        }

        /// <summary>
        /// このチップのマップオブジェクト2Dに対する相対的な拡大率を取得または設定する。
        /// </summary>
        public Vector2DF Scale
        {
            get { return CoreInstance.GetScale(); }
            set { CoreInstance.SetScale(value); }
        }

        /// <summary>
        /// このチップを描画する際の中心座標を取得または設定する。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get { return CoreInstance.GetCenterPosition(); }
            set { CoreInstance.SetCenterPosition(value); }
        }

        /// <summary>
        /// このチップのテクスチャを描画する際に合成する色を取得または設定する。
        /// </summary>
        public Color Color
        {
            get { return CoreInstance.GetColor(); }
            set { CoreInstance.SetColor(value); }
        }

        /// <summary>
        /// このチップのテクスチャ描画の左右反転フラグを取得または設定する。
        /// </summary>
        public bool TurnLR
        {
            get { return CoreInstance.GetTurnLR(); }
            set { CoreInstance.SetTurnLR(value); }
        }

        /// <summary>
        /// このチップのテクスチャ描画の上下反転フラグを取得または設定する。
        /// </summary>
        public bool TurnUL
        {
            get { return CoreInstance.GetTurnUL(); }
            set { CoreInstance.SetTurnUL(value); }
        }

        /// <summary>
        /// このチップのブレンディング モードを取得または設定する。
        /// </summary>
        public AlphaBlendMode AlphaBlendMode
        {
            get { return (AlphaBlendMode)CoreInstance.GetAlphaBlendMode(); }
            set { CoreInstance.SetAlphaBlendMode((swig.AlphaBlendMode)value); }
        }

        /// <summary>
        /// このチップのテクスチャを描画する際のテクスチャフィルタの種類を取得または設定する。
        /// </summary>
        public TextureFilterType TextureFilterType
        {
            get { return (TextureFilterType)CoreInstance.GetTextureFilterType(); }
            set { CoreInstance.SetTextureFilterType((swig.TextureFilterType)value); }
        }

        /// <summary>
        /// このチップの描画優先度を取得または設定する。
        /// </summary>
        public int DrawingPriority
        {
            get { return CoreInstance.GetDrawingPriority(); }
            set { CoreInstance.SetDrawingPriority(value); }
        }
    }
}
