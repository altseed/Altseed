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
				var weakPtr = Particular.Dictionary.Get(objects, id);
				T t = Particular.WeakReference.Get(weakPtr);

				if (t != null && t.IsReleased)
                {
					Particular.Dictionary.Set(objects, id, new WeakReference<T>(o));
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

        internal bool Contains(IntPtr id)
        {
            return GetObject(id) != null;
        }

        internal T GetObject(IntPtr id)
        {
            WeakReference<T> w = null;

            if (objects.TryGetValue(id, out w))
            {
                T t = null;

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
				var weakPtr = kv.Value;
				T t = Particular.WeakReference.Get(weakPtr);

                if (t != null || t.IsReleased)
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
				var weakPtr = kv.Value;
				T t = Particular.WeakReference.Get(weakPtr);

				if (t != null || !t.IsReleased)
                {
                    t.ForceToRelease();
                }
            }

            objects.Clear();
        }
    }
}
