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
            set { coreRectangle = value as swig.CoreRectangleShape; }
        }
        private swig.CoreRectangleShape coreRectangle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.RectangleShape; }
        }

        public RectangleShape()
        {
            coreRectangle = Engine.ObjectSystemFactory.CreateRectangleShape();

            var p = coreRectangle.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Shapes.AddObject(p, this);
        }

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
