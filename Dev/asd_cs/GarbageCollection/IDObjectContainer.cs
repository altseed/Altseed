using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace asd
{
    class IDObjectContainer<T> where T : class, IReleasable
    {
        Dictionary<IntPtr, WeakReference<T>> objects = new Dictionary<IntPtr, WeakReference<T>>();

        List<IntPtr> removingKeys = new List<IntPtr>();

        internal void AddObject(IntPtr id, T o)
        {
            if (o == null)
            {
                throw new ArgumentNullException("o");
            }

            if (objects.ContainsKey(id))
            {
                T t;

                if (!objects[id].TryGetTarget(out t) || t.IsReleased)
                {
                    objects[id] = new WeakReference<T>(o);
                    return;
                }

                if (t == o)
                {
                    Particular.Helper.ThrowException("二重登録です。");
                }
                else
                {
                    Particular.Helper.ThrowException("違うインスタンスが登録されています。");
                }
            }
            else
            {
                objects.Add(id, new WeakReference<T>(o));
            }
        }

        internal T GetObject(IntPtr id)
        {
            WeakReference<T> w;

            if (objects.TryGetValue(id, out w))
            {
                T t;

                if (w.TryGetTarget(out t) && !t.IsReleased)
                {
                    return t;
                }
            }

            return null;
        }

        internal void Collect()
        {
            removingKeys.Clear();

            foreach (var kv in objects)
            {
                T t;

                if (!kv.Value.TryGetTarget(out t) || t.IsReleased)
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

        internal void DestroyAll()
        {
            foreach (var kv in objects)
            {
                T t;

                if (kv.Value.TryGetTarget(out t) && !t.IsReleased)
                {
                    t.ForceToRelease();
                }
            }

            objects.Clear();
        }
    }
}
