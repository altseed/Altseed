using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;

namespace asd
{
    /// <summary>
    /// ポストエフェクトを適用するクラス
    /// </summary>
    public class PostEffect : IDisposable, IReleasable
    {
        swig.CorePostEffect coreInstance;

        public PostEffect()
        {
            coreInstance = Engine.ObjectSystemFactory.CreatePostEffect();

            var p = coreInstance.GetPtr();

            if (GC.PostEffects.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.PostEffects.AddObject(p, this);

			IsEnabled = true;
        }

        #region IDisposable Support
        bool disposed = false;

        ~PostEffect()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
			Particular.GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposed)
            {
                return;
            }

            var core = coreInstance;

            if (core != null)
            {
                GC.Collector.AddObject(core);
                coreInstance = null;
            }

            disposed = true;
        }
        #endregion

        #region GC対応
		[Browsable(false)]
		[EditorBrowsable(EditorBrowsableState.Never)]
        public bool IsReleased
        {
            get { return disposed; }
        }

        /// <summary>
        /// 強制的に使用しているメモリを開放する。
        /// </summary>
        /// <remarks>
        /// 何らかの理由でメモリが不足した場合に実行する。
        /// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
        /// </remarks>
		[Browsable(false)]
		[EditorBrowsable(EditorBrowsableState.Never)]
        public void ForceToRelease()
        {
            Dispose();
        }
        #endregion

        internal swig.CorePostEffect CoreInstance
        {
            get
            {
                if (disposed)
                {
                    throw new ObjectDisposedException(GetType().FullName);
                }

                return coreInstance;
            }
        }

        internal void Draw(RenderTexture2D dst, RenderTexture2D src)
        {
            if (disposed)
            {
                throw new ObjectDisposedException(GetType().FullName);
            }

            OnDraw(dst, src);
        }

        /**
        @brief	
        */
        /// <summary>
        /// オーバーライドして、毎フレーム描画される処理を記述できる。
        /// </summary>
        protected virtual void OnDraw(RenderTexture2D dst, RenderTexture2D src) { }

        /// <summary>
        /// マテリアルを用いてテクスチャに画像を描画する。
        /// </summary>
        /// <param name="target">描画先</param>
        /// <param name="material">マテリアル</param>
        protected void DrawOnTexture2DWithMaterial(RenderTexture2D target, Material2D material)
        {
            if (disposed)
            {
                throw new ObjectDisposedException(GetType().FullName);
            }

            coreInstance.DrawOnTexture2DWithMaterial(IG.GetRenderTexture2D(target), IG.GetMaterial2D(material));
        }

		/// <summary>
		/// このポストエフェクトが有効かどうか、取得、設定する。
		/// </summary>
		public bool IsEnabled { get; set; }
    }
}
