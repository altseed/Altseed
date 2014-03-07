using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class SoundSource : IDestroy
	{
		internal swig.SoundSource SwigObject;

		internal SoundSource(swig.SoundSource swig)
		{
#if DEBUG
			// 唯一の対応するクラスであることを保証
			if (GC.SoundSources.GetObject(swig.GetPtr()) != null) throw new Exception();
#endif
			SwigObject = swig;
		}

		~SoundSource()
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

		public void SetLoopPoint(float loopStart, float loopEnd)
		{
			SwigObject.SetLoopPoint(loopStart, loopEnd);
		}

		public float GetLength()
		{
			float ret = SwigObject.GetLength();
			return ret;
		}
	}
}
