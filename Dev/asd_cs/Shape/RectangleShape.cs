using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{

    /// <summary>
    /// 図形としての矩形を扱うクラス。
    /// </summary>
    public class RectangleShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreRectangle; }
        }
        private swig.CoreRectangleShape coreRectangle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.RectangleShape; }
        }

        public RectangleShape()
            : base()
        {
            coreRectangle = Engine.ObjectSystemFactory.CreateRectangleShape();

            var p = coreRectangle.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
				Particular.Helper.ThrowException("");
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~RectangleShape()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreRectangle == null; }
        }

		/// <summary>
		/// 強制的に使用しているメモリを開放する。
		/// </summary>
		/// <remarks>
		/// 何らかの理由でメモリが不足した場合に実行する。
		/// 開放した後の動作の保証はしていないので、必ず参照が残っていないことを確認する必要がある。
		/// </remarks>
        public override void Destroy()
        {
            lock (this)
            {
                if (coreRectangle == null) return;
                GC.Collector.AddObject(coreRectangle);
                coreRectangle = null;
            }
            Particular.GC.SuppressFinalize(this);
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
