using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// ポストエフェクトを適用するクラス
    /// </summary>
    public class PostEffect : IReleasable
    {
        internal swig.CorePostEffect SwigObject;

        public PostEffect()
        {
            SwigObject = Engine.ObjectSystemFactory.CreatePostEffect();

            var p = SwigObject.GetPtr();
            if (GC.PostEffects.GetObject(p) != null)
            {
                Particular.Helper.ThrowException("");
            }
            GC.PostEffects.AddObject(p, this);
        }

        #region GC対応
        ~PostEffect()
        {
            ForceToRelease();
        }

        public bool IsReleased
        {
            get { return SwigObject == null; }
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
                if (SwigObject == null) return;
                GC.Collector.AddObject(SwigObject);
                SwigObject = null;
            }
            Particular.GC.SuppressFinalize(this);
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
