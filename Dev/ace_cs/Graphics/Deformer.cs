using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Deformer : IDestroy
	{
		internal swig.Deformer SwigObject { get; set; }

		internal Deformer(swig.Deformer swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Deformer()
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

		public void AddBone(string name, int parentBoneIndex, RotationOrder rotationOrder, ref ace.Matrix44 localMat, ref ace.Matrix44 globalMatInv)
		{
			SwigObject.AddBone(name, parentBoneIndex, (swig.eRotationOrder)rotationOrder, ref localMat, ref globalMatInv);
		}
	}
}
