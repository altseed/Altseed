
#include "asd.Sound_Imp.h"
#include "asd.SoundSource_Imp.h"

namespace asd
{
	SoundSource_Imp::SoundSource_Imp(Sound* manager, osm::Sound* sound, bool isDecompressed)
		: m_isDecompressed(isDecompressed)
	{
		m_manager = manager;
		m_sound = sound;

		SafeAddRef(m_manager);
	}

	SoundSource_Imp::~SoundSource_Imp()
	{
		auto s = (Sound_Imp*) m_manager;
		s->SoundSourcesContainer->Unregister(this);

		SafeRelease(m_sound);
		SafeRelease(m_manager);
	}

	float SoundSource_Imp::GetLength()
	{
		return m_sound->GetLength();
	}

	bool SoundSource_Imp::GetIsPlaybackSpeedEnabled()
	{
		return m_sound->GetIsPlaybackSpeedEnabled();
	}

	void SoundSource_Imp::SetIsPlaybackSpeedEnabled(bool isPlaybackSpeedEnabled)
	{
		m_sound->SetIsPlaybackSpeedEnabled(isPlaybackSpeedEnabled);
	}

	float SoundSource_Imp::GetPlaybackSpeed()
	{
		return m_sound->GetPlaybackSpeed();
	}

	void SoundSource_Imp::SetPlaybackSpeed(float playbackSpeed)
	{
		m_sound->SetPlaybackSpeed(playbackSpeed);
	}

	float SoundSource_Imp::GetPanningPosition()
	{
		return m_sound->GetPanningPosition();
	}

	void SoundSource_Imp::SetPanningPosition(float panningPosition)
	{
		m_sound->SetPanningPosition(panningPosition);
	}

	void SoundSource_Imp::Reload(uint8_t* data, int32_t size)
	{
		auto ls = GetLoopStartingPoint();
		auto le = GetLoopEndPoint();
		auto lm = GetIsLoopingMode();

		auto s = (Sound_Imp*) m_manager;
		auto source = s->GetManager()->CreateSound(data, size, m_isDecompressed);
		if (source == nullptr) return;

		SafeRelease(m_sound);
		m_sound = source;

		SetLoopStartingPoint(ls);
		SetLoopEndPoint(le);
		SetIsLoopingMode(lm);
	}
}