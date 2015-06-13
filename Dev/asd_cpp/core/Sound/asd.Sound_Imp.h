
#pragma once

#include "asd.Sound.h"
#include "../asd.ReferenceObject.h"

#include "../Utils/asd.ResourceContainer.h"

#include <OpenSoundMixer.h>

#ifdef _WIN64

#if _DEBUG
#pragma comment(lib,"x64/Debug/libogg_static.lib")
#pragma comment(lib,"x64/Debug/libvorbis_static.lib")
#pragma comment(lib,"x64/Debug/libvorbisfile_static.lib")
#pragma comment(lib,"x64/Debug/OpenSoundMixer.lib")
#else
#pragma comment(lib,"x64/Release/libogg_static.lib")
#pragma comment(lib,"x64/Release/libvorbis_static.lib")
#pragma comment(lib,"x64/Release/libvorbisfile_static.lib")
#pragma comment(lib,"x64/Release/OpenSoundMixer.lib")
#endif

#else

#if _DEBUG
#pragma comment(lib,"x86/Debug/libogg_static.lib")
#pragma comment(lib,"x86/Debug/libvorbis_static.lib")
#pragma comment(lib,"x86/Debug/libvorbisfile_static.lib")
#pragma comment(lib,"x86/Debug/OpenSoundMixer.lib")
#else
#pragma comment(lib,"x86/Release/libogg_static.lib")
#pragma comment(lib,"x86/Release/libvorbis_static.lib")
#pragma comment(lib,"x86/Release/libvorbisfile_static.lib")
#pragma comment(lib,"x86/Release/OpenSoundMixer.lib")
#endif

#endif

namespace asd
{
	class Sound_Imp
		: public Sound
		, public ReferenceObject
	{
	private:
		osm::Manager*	m_manager;
		Log*			log;
		File*			m_file;

	public:

		Sound_Imp(File *file, Log* log, bool isReloadingEnabled);
		virtual ~Sound_Imp();

		SoundSource* CreateSoundSource_(const achar* path, bool isDecompressed) override;

		int32_t Play(SoundSource* soundSource) override;

		bool GetIsPlaying(int32_t id) override;

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