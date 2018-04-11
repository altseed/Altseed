
#include "asd.Sound_Imp.h"
#include "asd.SoundSource_Imp.h"

#include "../Log/asd.Log.h"

namespace asd
{
	Sound_Imp::Sound_Imp(File *file, Log* log, bool isReloadingEnabled)
		: m_manager(nullptr)
		, m_file(file)
		, log(log)
	{
		log->WriteHeading("音");

		m_manager = osm::Manager::Create();
		if (m_manager->Initialize())
		{
			log->WriteLine("音初期化成功");
		}
		else
		{
			log->WriteLine("音初期化失敗");
			SafeRelease(m_manager);
		}

		SoundSourcesContainer = std::make_shared<ResourceContainer<SoundSource_Imp>>(file);
	}

	Sound_Imp::~Sound_Imp()
	{
		if (m_manager != nullptr)
		{
			m_manager->Finalize();
		}

		SafeRelease(m_manager);
	}

	SoundSource* Sound_Imp::CreateSoundSource(const achar* path, bool isDecompressed)
	{
		if (m_manager == nullptr) return nullptr;

		auto ret = SoundSourcesContainer->TryLoad(path, [this, isDecompressed](uint8_t* data, int32_t size) -> SoundSource_Imp*
		{
			auto source = m_manager->CreateSound(data, size, isDecompressed);
			if (source == nullptr) return nullptr;
			return new SoundSource_Imp(this, source, isDecompressed);
		});

		return ret;
	}

	int32_t Sound_Imp::Play(SoundSource* soundSource)
	{
		if (m_manager == nullptr) return -1;
		if (soundSource == nullptr) return -1;
		auto s = (SoundSource_Imp*) soundSource;

		return m_manager->Play(s->GetSound());
	}

	bool Sound_Imp::GetIsPlaying(int32_t id)
	{
		if (m_manager == nullptr) return false;
		return m_manager->IsPlaying(id);
	}

	void Sound_Imp::StopAll()
	{
		if (m_manager == nullptr) return;
		return m_manager->StopAll();
	}

	void Sound_Imp::Stop(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Stop(id);
	}

	void Sound_Imp::Pause(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Pause(id);
	}

	void Sound_Imp::Resume(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Resume(id);
	}

	void Sound_Imp::SetVolume(int32_t id, float volume)
	{
		if (m_manager == nullptr) return;
		return m_manager->SetVolume(id, volume);
	}

	void Sound_Imp::FadeIn(int32_t id, float second)
	{
		if (m_manager == nullptr) return;
		return m_manager->FadeIn(id, second);
	}

	void Sound_Imp::FadeOut(int32_t id, float second)
	{
		if (m_manager == nullptr) return;
		return m_manager->FadeOut(id, second);
	}

	void Sound_Imp::Fade(int32_t id, float second, float targetedVolume)
	{
		if (m_manager == nullptr) return;
		return m_manager->Fade(id, second, targetedVolume);
	}

	bool Sound_Imp::GetIsPlaybackSpeedEnabled(int32_t id)
	{
		if (m_manager == nullptr) return false;
		return m_manager->GetIsPlaybackSpeedEnabled(id);
	}

	void Sound_Imp::SetIsPlaybackSpeedEnabled(int32_t id, bool isPlaybackSpeedEnabled)
	{
		if (m_manager == nullptr) return;
		m_manager->SetIsPlaybackSpeedEnabled(id, isPlaybackSpeedEnabled);
	}

	float Sound_Imp::GetPlaybackSpeed(int32_t id)
	{
		if (m_manager == nullptr) return 1.0f;
		return m_manager->GetPlaybackSpeed(id);
	}

	void Sound_Imp::SetPlaybackSpeed(int32_t id, float playbackSpeed)
	{
		if (m_manager == nullptr) return;
		m_manager->SetPlaybackSpeed(id, playbackSpeed);
	}

	float Sound_Imp::GetPanningPosition(int32_t id)
	{
		if (m_manager == nullptr) return 0.0f;
		return m_manager->GetPanningPosition(id);
	}

	void Sound_Imp::SetPanningPosition(int32_t id, float panningPosition)
	{
		if (m_manager == nullptr) return;
		m_manager->SetPanningPosition(id, panningPosition);
	}

	void Sound_Imp::Reload()
	{
		SoundSourcesContainer->Reload([this](std::shared_ptr<ResourceContainer<SoundSource_Imp>::LoadingInformation> r, uint8_t* data, int32_t size) -> void
		{
			r->ResourcePtr->Reload(data, size);
		});
	}
}