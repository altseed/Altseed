using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Line : Shape,IDestroy
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreLine; }
        }
        private swig.CoreLine coreLine { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.Line; }
        }

        public Line()
            : base()
        {
            coreLine = Engine.ObjectSystemFactory.CreateLine();

            var p = coreLine.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~Line()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreLine == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreLine == null) return;
                GC.Collector.AddObject(coreLine);
                coreLine = null;
            }
            System.GC.SuppressFinalize(this);
        }
        #endregion


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
