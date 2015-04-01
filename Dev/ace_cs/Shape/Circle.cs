using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Circle : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreCircle; }
        }
        private swig.CoreCircle coreCircle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.Circle; }
        }

        public Circle()
            : base()
        {
            coreCircle = Engine.ObjectSystemFactory.CreateCircle();

            var p = coreCircle.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~Circle()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreCircle == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreCircle == null) return;
                GC.Collector.AddObject(coreCircle);
                coreCircle = null;
            }
            System.GC.SuppressFinalize(this);
        }
        #endregion


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

        public float OuterDiameter
        {
            get
            {
                return coreCircle.GetOuterDiameter();
            }
            set
            {
                coreCircle.SetOuterDiamater(value);
            }
        }

        public float InnerDiameter
        {
            get
            {
                return coreCircle.GetInnerDiameter();
            }
            set
            {
                coreCircle.SetInnerDiamater(value);
            }
        }


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
