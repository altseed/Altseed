using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public enum ShapeType
    {
        Arc = swig.ShapeType.Arc,
        Circle = swig.ShapeType.Circle,
        Rectangle = swig.ShapeType.Rectangle,
        Line = swig.ShapeType.Line,
        Triangle = swig.ShapeType.Triangle,
    }

    public abstract class Shape : IDestroy
    {
        public abstract ShapeType ShapeType { get; }

        internal abstract swig.CoreShape CoreShape { get; }

        public abstract bool IsDestroyed { get; }

        public abstract void Destroy();
    }
}
