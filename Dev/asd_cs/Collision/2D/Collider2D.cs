using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;


namespace asd
{
    /// <summary>
    /// 円形のコライダクラス
    /// </summary>
    public abstract class Collider2D : IDisposable,IReleasable
    {

        #region IDisposable Support
        bool disposed = false;

        ~Collider2D()
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

            var core = CoreCollider;

            if (core != null)
            {
                GC.Collector.AddObject(core);
                CoreCollider = null;
            }

            disposed = true;
        }
        #endregion

        internal abstract swig.CoreCollider2D CoreCollider { get; set; }

        /// <summary>
        /// コライダの形状を取得
        /// </summary>
        public abstract CoreCollider2DShapeType CoreCollider2DShapeType { get; }

        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        public bool IsReleased
        {
            get { return disposed; }
        }

        [Browsable(false)]
        [EditorBrowsable(EditorBrowsableState.Never)]
        public void ForceToRelease()
        {
            Dispose();
        }

        /// <summary>
        /// このコライダを保持しているObject2Dを取得する
        /// </summary>
        public Object2D OwnerObject { get; internal set; }

        /// <summary>
        /// このコライダのタグを取得、または設定する
        /// </summary>
        public string Tag
        {
            get
            {
                return CoreCollider.GetTag();
            }
            set
            {
                CoreCollider.SetTag(value);
            }
        }

        /// <summary>
        /// このコライダを可視化する成否を取得、または設定する
        /// </summary>
        public bool IsVisible
        {
            get
            {
                return CoreCollider.GetIsVisible();
            }
            set
            {
                CoreCollider.SetIsVisible(value);
            }
        }
    }
}