﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    /// <summary>
    /// 図形としての円を扱うクラス。
    /// </summary>
    public class CircleShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreCircle; }
            set { coreCircle = value as swig.CoreCircleShape; }
        }
        private swig.CoreCircleShape coreCircle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.CircleShape; }
        }

        public CircleShape()
        {
            coreCircle = Engine.ObjectSystemFactory.CreateCircleShape();

            var p = coreCircle.GetPtr();

            if (GC.Shapes.Contains(p))
            {
                Particular.Helper.ThrowException("");
            }

            GC.Shapes.AddObject(p, this);
        }

        /// <summary>
        /// 円の中心座標を取得または設定する。
        /// </summary>
        public Vector2DF Position
        {
            get
            {
                return coreCircle.GetPosition();
            }
            set
            {
                coreCircle.SetPosition(value);
            }
        }

        /// <summary>
        /// 円の外径を取得または設定する。
        /// </summary>
        public float OuterDiameter
        {
            get
            {
                return coreCircle.GetOuterDiameter();
            }
            set
            {
                coreCircle.SetOuterDiameter(value);
            }
        }

        /// <summary>
        /// 円の内径を取得または設定する。
        /// </summary>
        public float InnerDiameter
        {
            get
            {
                return coreCircle.GetInnerDiameter();
            }
            set
            {
                coreCircle.SetInnerDiameter(value);
            }
        }

        /// <summary>
        /// 円の中心を軸とした回転角度を取得または設定する。
        /// </summary>
        public float Angle
        {
            get
            {
                return coreCircle.GetAngle();
            }
            set
            {
                coreCircle.SetAngle(value);
            }
        }

        /// <summary>
        /// 円の外周、内周上の曲がり角（滑らかさ）の数を取得または設定する。
        /// </summary>
        public int NumberOfCorners
        {
            get
            {
                return coreCircle.GetNumberOfCorners();
            }
            set
            {
                coreCircle.SetNumberOfCorners(value);
            }
        }
    }
}
