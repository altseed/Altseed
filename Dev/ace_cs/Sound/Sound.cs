using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ace
{
	public class Sound
	{
		internal swig.Sound SwigObject = null;

		internal Sound(swig.Sound sound)
		{
			SwigObject = sound;
		}

		public SoundSource CreateSoundSource(string path, bool isDecompressed)
		{
			return GC.GenerateSoundSource(swig.Accessor.Sound_CreateSoundSource(SwigObject, path, isDecompressed), GC.GenerationType.Create);
		}

		public int Play(SoundSource soundSource)
		{
			int ret = SwigObject.Play(soundSource.SwigObject);
			return ret;
		}

		public bool IsPlaying(int id)
		{
			bool ret = SwigObject.IsPlaying(id);
			return ret;
		}

		public void StopAll()
		{
			SwigObject.StopAll();
		}

		public void Stop(int id)
		{
			SwigObject.Stop(id);
		}

		public void Pause(int id)
		{
			SwigObject.Pause(id);
		}

		public void Resume(int id)
		{
			SwigObject.Resume(id);
		}

		public void SetVolume(int id, float volume)
		{
			SwigObject.SetVolume(id, volume);
		}

		public void FadeIn(int id, float second)
		{
			SwigObject.FadeIn(id, second);
		}

		public void FadeOut(int id, float second)
		{
			SwigObject.FadeOut(id, second);
		}
	}
}
