using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class Triangle : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return coreTriangle; }
        }
        private swig.CoreTriangle coreTriangle { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.Triangle; }
        }

        public Triangle()
            : base()
        {
            coreTriangle = Engine.ObjectSystemFactory.CreateTriangle();

            var p = coreTriangle.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~Triangle()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return coreTriangle == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (coreTriangle == null) return;
                GC.Collector.AddObject(coreTriangle);
                coreTriangle = null;
            }
            System.GC.SuppressFinalize(this);
        }
        #endregion


        public Vector2DF GetPointByIndex(int index)
        {
            return coreTriangle.GetPointByIndex(index);
        }

		public void SetPointByIndex(Vector2DF point, int index)
        {
            coreTriangle.SetPointByIndex(point, index);
        }

		public Vector2DF GetUVByIndex(int index)
        {
            return coreTriangle.GetUVByIndex(index);
        }

		public void SetUVByIndex(Vector2DF uv, int index)
        {
            coreTriangle.SetUVByIndex(uv, index);
        }
    }
}
