using asd.swig;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 線形のコライダクラス
    /// </summary>
    public class LineCollider : Collider2D
    {
        internal override CoreCollider2D CoreCollider
        {
            get
            {
                return coreCollider;
            }

            set
            {
                coreCollider = value as swig.CoreLineCollider;
            }
        }

        private swig.CoreLineCollider coreCollider { get; set; }


        public LineCollider()
        {
            coreCollider = Engine.ObjectSystemFactory.CreateLineCollider();

            var p = coreCollider.GetPtr();

            if (GC.Collider2Ds.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Collider2Ds.AddObject(p, this);
        }

        /// <summary>
        /// 線分の始点座標を取得または設定する
        /// </summary>
        public Vector2DF StartingPosition
        {
            get
            {
                return coreCollider.GetStartingPosition();
            }
            set
            {
                coreCollider.SetStartingPosition(value);
            }
        }

        /// <summary>
        /// 線分の終点座標を取得または設定する
        /// </summary>
        public Vector2DF EndingPosition
        {
            get
            {
                return coreCollider.GetEndingPosition();
            }
            set
            {
                coreCollider.SetEndingPosition(value);
            }
        }

        /// <summary>
        /// 線分の太さを取得または設定する
        /// </summary>
        public float Thickness
        {
            get
            {
                return coreCollider.GetThickness();
            }
            set
            {
                coreCollider.SetThickness(value);
            }
        }

        public override CoreCollider2DShapeType CoreCollider2DShapeType
        {
            get
            {
                return CoreCollider2DShapeType.Line;
            }
        }
    }
}