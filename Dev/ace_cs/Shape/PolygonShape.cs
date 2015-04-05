using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class PolygonShape : Shape
    {
        internal override swig.CoreShape CoreShape
        {
            get { return corePolygon; }
        }
        private swig.CorePolygonShape corePolygon { get; set; }

        public override ShapeType ShapeType
        {
            get { return ShapeType.PolygonShape; }
        }

        public PolygonShape()
            : base()
        {
            corePolygon = Engine.ObjectSystemFactory.CreatePolygonShape();

            var p = corePolygon.GetPtr();
            if (GC.Shapes.GetObject(p) != null)
            {
                throw new Exception();
            }
            GC.Shapes.AddObject(p, this);
        }

        #region GC対応
        ~PolygonShape()
        {
            Destroy();
        }

        public override bool IsDestroyed
        {
            get { return corePolygon == null; }
        }

        public override void Destroy()
        {
            lock (this)
            {
                if (corePolygon == null) return;
                GC.Collector.AddObject(corePolygon);
                corePolygon = null;
            }
            System.GC.SuppressFinalize(this);
        }
        #endregion

        void AddVertex(Vector2DF vertex)
        {
            corePolygon.AddVertex(vertex);
        }

        void ClearVertexes()
        {
            corePolygon.ClearVertexes();
        }

        int GetVertexesNum()
        {
            return corePolygon.GetVertexesNum();
        }
    }
}
