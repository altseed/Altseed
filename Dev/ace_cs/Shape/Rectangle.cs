using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Rectangle : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreRectangle; }
        }
        private swig.CoreRectangle coreRectangle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.Rectangle; }
        }

        public Rectangle()
            : base()
        {
            coreRectangle = Engine.ObjectSystemFactory.CreateRectangle();

            var p = coreRectangle.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~Rectangle()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreRectangle == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreRectangle == null) return;
                GC.Collector.AddObject(coreRectangle);
                coreRectangle = null;
            }
            System.GC.SuppressFinalize(this);
        }
        #endregion
		
        /// <summary>
        /// 矩形の描画範囲を取得もしくは設定する。
        /// </summary>
        public RectF DrawingArea
        {
            get
            {
                return coreRectangle.GetDrawingArea();
            }
            set
            {
                coreRectangle.SetDrawingArea(value);
            }
        }

        /// <summary>
        /// 矩形の頂点別UVを取得もしくは設定する。
        /// </summary>
        public RectF UV
        {
            get
            {
                return coreRectangle.GetUV();
            }
            set
            {
                coreRectangle.SetUV(value);
            }
        }

        /// <summary>
        /// 矩形の回転角度を取得もしくは設定する。
        /// </summary>
        public float Angle
        {
            get
            {
                return coreRectangle.GetAngle();
            }
            set
            {
                coreRectangle.SetAngle(value);
            }
        }

        /// <summary>
        /// 矩形の回転の際の中心座標を取得もしくは設定する。
        /// </summary>
        public Vector2DF CenterPosition
        {
            get
            {
                return coreRectangle.GetCenterPosition();
            }
            set
            {
                coreRectangle.SetCenterPosition(value);
            }
        }
    }
}
