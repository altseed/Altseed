using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 図形としての弧を扱うクラス。
    /// </summary>
    public class ArcShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreArc; }
            set { coreArc = value as swig.CoreArcShape; }
        }
        private swig.CoreArcShape coreArc { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.ArcShape; }
        }

        public ArcShape()
        {
            coreArc = Engine.ObjectSystemFactory.CreateArcShape();

            var p = coreArc.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Shapes.AddObject(p, this);
        }

        /// <summary>
        /// 弧の中心座標を取得または設定する。
        /// </summary>
        public Vector2DF Position
        {
            get
            {
                return coreArc.GetPosition();
            }
            set
            {
                coreArc.SetPosition(value);
            }
        }

        /// <summary>
        /// 弧の外径を取得または設定する。
        /// </summary>
        public float OuterDiameter
        {
            get
            {
                return coreArc.GetOuterDiameter();
            }
            set
            {
                coreArc.SetOuterDiameter(value);
            }
        }


        /// <summary>
        /// 弧の内径を取得または設定する。
        /// </summary>
        public float InnerDiameter
        {
            get
            {
                return coreArc.GetInnerDiameter();
            }
            set
            {
                coreArc.SetInnerDiameter(value);
            }
        }

        /// <summary>
        /// 弧の中心を軸とした回転角度を取得または設定する。
        /// </summary>
        public float Angle
        {
            get
            {
                return coreArc.GetAngle();
            }
            set
            {
                coreArc.SetAngle(value);
            }
        }

        /// <summary>
        /// 弧の外周、内周上の曲がり角（滑らかさ）の数を取得または設定する。
        /// </summary>
        public int NumberOfCorners
        {
            get
            {
                return coreArc.GetNumberOfCorners();
            }
            set
            {
                coreArc.SetNumberOfCorners(value);
            }
        }

        /// <summary>
        /// 弧の 始点となる外周、内周の曲がり角の番号を取得または設定する。(0 &lt;= StartingCorner &lt;= NumberOfCorners)
        /// </summary>
        public int StartingCorner
        {
            get
            {
                return coreArc.GetStartingCorner();
            }
            set
            {
                coreArc.SetStartingCorner(value);
            }
        }

        /// <summary>
        /// 弧の終点となる外周、内周の曲がり角の番号を取得または設定する。(0 &lt;= EndingCorner &lt;= NumberOfCorners)
        /// </summary>
        public int EndingCorner
        {
            get
            {
                return coreArc.GetEndingCorner();
            }
            set
            {
                coreArc.SetEndingCorner(value);
            }
        }
    }
}
