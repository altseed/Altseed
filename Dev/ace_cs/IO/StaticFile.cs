using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
    public class StaticFile : IDestroy
    {
        internal swig.StaticFile SwigObject;

        internal StaticFile(swig.StaticFile swig)
        {
            if (GC.StaticFiles.GetObject(swig.GetPtr()) != null) throw new Exception();

            SwigObject = swig;
        }

        ~StaticFile()
        {
            Destroy();
        }


        public bool IsDestroyed
        {
            get
            {
                return SwigObject == null;
            }
        }

        public void Destroy()
        {
            lock (this)
            {
                if (SwigObject == null) return;
                GC.Collector.AddObject(SwigObject);
                SwigObject = null;
            }
            System.GC.SuppressFinalize(this);
        }

        public List<byte> ReadAllBytes()
        {
            List<byte> bytes = new List<byte>();

            return bytes;
        }
    }
}
