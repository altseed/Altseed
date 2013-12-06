using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Mesh : IDestroy
	{
		internal swig.Mesh SwigObject { get; set; }

		internal Mesh(swig.Mesh swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.Meshs.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~Mesh()
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

		public void AddVertex(ace.Vector3DF position, ace.Vector3DF normal, ace.Vector3DF binormal, ace.Vector2DF uv1, ace.Vector2DF uv2, ace.Color color, int boneWeights, int boneIndexes)
		{
			SwigObject.AddVertex(ref position, ref normal, ref binormal, ref uv1, ref uv2, ref color, boneWeights, boneIndexes);
		}

		public virtual void AddFace(int index1, int index2, int index3)
		{
			SwigObject.AddFace(index1, index2, index3);
		}

		public virtual void AddMaterialOffset(int materialIndex, int faceOffset)
		{
			SwigObject.AddMaterialOffset(materialIndex, faceOffset);
		}

		public virtual void SendToGPUMemory()
		{
			SwigObject.SendToGPUMemory();
		}
	}
}
