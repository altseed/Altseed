
#include "asd.Sound.h"
#include "asd.SoundSource.h"

#include "../Log/asd.Log.h"

namespace asd
{
	Sound::Sound(File *file, Log* log, bool isReloadingEnabled)
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

		SoundSourcesContainer = std::make_shared<ResourceContainer<SoundSource>>(file);
	}

	Sound::~Sound()
	{
		if (m_manager != nullptr)
		{
			m_manager->Finalize();
		}

		SafeRelease(m_manager);
	}

	SoundSource* Sound::CreateSoundSource(const achar* path, bool isDecompressed)
	{
		if (m_manager == nullptr) return nullptr;

		auto ret = SoundSourcesContainer->TryLoad(path, [this, isDecompressed](uint8_t* data, int32_t size) -> SoundSource*
		{
			auto source = m_manager->CreateSound(data, size, isDecompressed);
			if (source == nullptr) return nullptr;
			return new SoundSource(this, source, isDecompressed);
		});

		return ret;
	}

	int32_t Sound::Play(SoundSource* soundSource)
	{
		if (m_manager == nullptr) return -1;
		if (soundSource == nullptr) return -1;
		auto s = (SoundSource*) soundSource;

		return m_manager->Play(s->GetSound());
	}

	bool Sound::GetIsPlaying(int32_t id)
	{
		if (m_manager == nullptr) return false;
		return m_manager->IsPlaying(id);
	}

	void Sound::StopAll()
	{
		if (m_manager == nullptr) return;
		return m_manager->StopAll();
	}

	void Sound::Stop(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Stop(id);
	}

	void Sound::Pause(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Pause(id);
	}

	void Sound::Resume(int32_t id)
	{
		if (m_manager == nullptr) return;
		return m_manager->Resume(id);
	}

	void Sound::SetVolume(int32_t id, float volume)
	{
		if (m_manager == nullptr) return;
		return m_manager->SetVolume(id, volume);
	}

	void Sound::FadeIn(int32_t id, float second)
	{
		if (m_manager == nullptr) return;
		return m_manager->FadeIn(id, second);
	}

	void Sound::FadeOut(int32_t id, float second)
	{
		if (m_manager == nullptr) return;
		return m_manager->FadeOut(id, second);
	}

	void Sound::Fade(int32_t id, float second, float targetedVolume)
	{
		if (m_manager == nullptr) return;
		return m_manager->Fade(id, second, targetedVolume);
	}

	bool Sound::GetIsPlaybackSpeedEnabled(int32_t id)
	{
		if (m_manager == nullptr) return false;
		return m_manager->GetIsPlaybackSpeedEnabled(id);
	}

	void Sound::SetIsPlaybackSpeedEnabled(int32_t id, bool isPlaybackSpeedEnabled)
	{
		if (m_manager == nullptr) return;
		m_manager->SetIsPlaybackSpeedEnabled(id, isPlaybackSpeedEnabled);
	}

	float Sound::GetPlaybackSpeed(int32_t id)
	{
		if (m_manager == nullptr) return 1.0f;
		return m_manager->GetPlaybackSpeed(id);
	}

	void Sound::SetPlaybackSpeed(int32_t id, float playbackSpeed)
	{
		if (m_manager == nullptr) return;
		m_manager->SetPlaybackSpeed(id, playbackSpeed);
	}

	float Sound::GetPanningPosition(int32_t id)
	{
		if (m_manager == nullptr) return 0.0f;
		return m_manager->GetPanningPosition(id);
	}

	void Sound::SetPanningPosition(int32_t id, float panningPosition)
	{
		if (m_manager == nullptr) return;
		m_manager->SetPanningPosition(id, panningPosition);
	}

	float Sound::GetPlaybackPercent(int32_t id)
	{
		if (m_manager == nullptr)
			return;
		return m_manager->GetPlaybackPercent(id);
	}

	void Sound::Reload()
	{
		SoundSourcesContainer->Reload([this](std::shared_ptr<ResourceContainer<SoundSource>::LoadingInformation> r, uint8_t* data, int32_t size) -> void
		{
			r->ResourcePtr->Reload(data, size);
		});
	}
}