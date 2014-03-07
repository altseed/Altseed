
#pragma once

#include "../ace.Core.Base.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class Sound
		: public IReference
	{
		friend class Accessor;

	protected:
		Sound() {}
		~Sound() {}

		virtual SoundSource* CreateSoundSource_(const achar* path, bool isDecompressed) = 0;
	public:
		
#if !SWIG
		std::shared_ptr<SoundSource> CreateSoundSource(const achar* path, bool isDecompressed)
		{
			return CreateSharedPtrWithReleaseDLL(CreateSoundSource_(path, isDecompressed));
		}
#endif

		virtual int32_t Play(SoundSource* soundSource) = 0;

#if !SWIG
		int32_t Play(std::shared_ptr<SoundSource> soundSource)
		{
			return Play(soundSource.get());
		}
#endif

		virtual bool IsPlaying(int32_t id) = 0;

		virtual void StopAll() = 0;

		virtual void Stop(int32_t id) = 0;

		virtual void Pause(int32_t id) = 0;

		virtual void Resume(int32_t id) = 0;

		virtual void SetVolume(int32_t id, float volume) = 0;

		virtual void FadeIn(int32_t id, float second) = 0;

		virtual void FadeOut(int32_t id, float second) = 0;
	};
}