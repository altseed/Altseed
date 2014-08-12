
#pragma once

#include "ace.Sound.h"
#include "../ace.ReferenceObject.h"

#include "../Utils/ace.ResourceContainer.h"

#include <OpenSoundMixer.h>

#if _DEBUG
#pragma comment(lib,"Debug/libogg_static.lib")
#pragma comment(lib,"Debug/libvorbis_static.lib")
#pragma comment(lib,"Debug/libvorbisfile_static.lib")
#pragma comment(lib,"Debug/OpenSoundMixer.lib")
#else
#pragma comment(lib,"Release/libogg_static.lib")
#pragma comment(lib,"Release/libvorbis_static.lib")
#pragma comment(lib,"Release/libvorbisfile_static.lib")
#pragma comment(lib,"Release/OpenSoundMixer.lib")
#endif

namespace ace
{
	class Sound_Imp
		: public Sound
		, public ReferenceObject
	{
	private:
		osm::Manager*	m_manager;

	public:

		Sound_Imp(bool isReloadingEnabled);
		virtual ~Sound_Imp();

		SoundSource* CreateSoundSource_(const achar* path, bool isDecompressed) override;

		int32_t Play(SoundSource* soundSource) override;

		bool IsPlaying(int32_t id) override;

		void StopAll() override;

		void Stop(int32_t id) override;

		void Pause(int32_t id) override;

		void Resume(int32_t id) override;

		void SetVolume(int32_t id, float volume) override;

		void FadeIn(int32_t id, float second) override;

		void FadeOut(int32_t id, float second) override;

#if !SWIG
		std::shared_ptr<ResourceContainer<SoundSource_Imp>>	SoundSourcesContainer;

		void Reload();

		osm::Manager* GetManager() { return m_manager; }
#endif
		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}