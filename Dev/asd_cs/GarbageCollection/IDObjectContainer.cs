using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    class IDObjectContainer<T> where T : class, IReleasable
    {
        Dictionary<IntPtr, WeakReference> objects = new Dictionary<IntPtr, WeakReference>();

        List<IntPtr> removingKeys = new List<IntPtr>();

        public void AddObject(IntPtr id, T o)
        {
            if (objects.ContainsKey(id))
            {
                T t = null;
                t = objects[id].Target as T;

                if (t == null ||
                    t.IsReleased)
                {
                    objects[id] = new WeakReference(o);
                    return;
                }

                if (t == o)
                {
                    Particular.Helper.ThrowException("二重登録です。");
                }

                if (t != o)
                {
                    Particular.Helper.ThrowException("違うインスタンスが登録されています。");
                }
            }
            else
            {
                objects.Add(id, new WeakReference(o));
            }
        }

        public T GetObject(IntPtr id)
        {
            WeakReference w = null;
            if (objects.TryGetValue(id, out w))
            {
                var t = (T)w.Target;
                if (t != null && !t.IsReleased) return t;
            }

            return null;
        }

        public void Collect()
        {
            removingKeys.Clear();

            foreach (var kv in objects)
            {
                var t = (T)kv.Value.Target;
                if (t == null ||
                    t.IsReleased)
                {
                    removingKeys.Add(kv.Key);
                }
            }

            foreach (var r in removingKeys)
            {
                objects.Remove(r);
            }

            removingKeys.Clear();
        }

        public void DestroyAll()
        {
            foreach (var kv in objects)
            {
                var t = (T)kv.Value.Target;
                if (t == null ||
                    t.IsReleased)
                {
                    continue;
                }
                t.ForceToRelease();
            }

            objects.Clear();
        }
    }
}
