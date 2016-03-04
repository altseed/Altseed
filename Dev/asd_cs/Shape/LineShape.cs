using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{

    /// <summary>
    /// 図形としての線分を扱うクラス。
    /// </summary>
    public class LineShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreLine; }
            set { coreLine = value as swig.CoreLineShape; }
        }
        private swig.CoreLineShape coreLine { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.LineShape; }
        }

        public LineShape()
        {
            coreLine = Engine.ObjectSystemFactory.CreateLineShape();

            var p = coreLine.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Shapes.AddObject(p, this);
        }

        /// <summary>
        /// 線分の始点座標を取得もしくは設定する。
        /// </summary>
        public Vector2DF StartingPosition
        {
            get
            {
                return coreLine.GetStartingPosition();
            }
            set
            {
                coreLine.SetStartingPosition(value);
            }
        }

        /// <summary>
        /// 線分の終点座標を取得もしくは設定する。
        /// </summary>
        public Vector2DF EndingPosition
        {
            get
            {
                return coreLine.GetEndingPosition();
            }
            set
            {
                coreLine.SetEndingPosition(value);
            }
        }

        /// <summary>
        /// 線分の太さを取得もしくは設定する。
        /// </summary>
        public float Thickness
        {
            get
            {
                return coreLine.GetThickness();
            }
            set
            {
                coreLine.SetThickness(value);
            }
        }
    }
}
