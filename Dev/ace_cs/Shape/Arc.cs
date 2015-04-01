using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Arc : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreArc; }
        }
        private swig.CoreArc coreArc { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.Arc; }
        }

        public Arc()
            :base()
		{
			coreArc = Engine.ObjectSystemFactory.CreateArc();

			var p = coreArc.GetPtr();
			if(GC.Shapes.GetObject(p) != null)
			{
				throw new Exception();
			}
			GC.Shapes.AddObject(p, this);
		}

		#region GC対応
        ~Arc()
		{
			Destroy();
		}

		public override bool IsDestroyed
		{
			get { return coreArc == null; }
		}

		public override void Destroy()
		{
			lock (this)
			{
				if(coreArc == null) return;
				GC.Collector.AddObject(coreArc);
				coreArc = null;
			}
			System.GC.SuppressFinalize(this);
		}
		#endregion

        		
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

        public float OuterDiameter
        {
            get
            {
                return coreArc.GetOuterDiameter();
            }
            set
            {
                coreArc.SetOuterDiamater(value);
            }
        }
        
        public float InnerDiameter
        {
            get
            {
                return coreArc.GetInnerDiameter();
            }
            set
            {
                coreArc.SetInnerDiamater(value);
            }
        }

                
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
