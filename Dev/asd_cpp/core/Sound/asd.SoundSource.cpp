
#include "asd.Sound.h"
#include "asd.SoundSource.h"

namespace asd
{
	SoundSource::SoundSource(Sound* manager, osm::Sound* sound, bool isDecompressed)
		: m_isDecompressed(isDecompressed)
	{
		m_manager = manager;
		m_sound = sound;

		SafeAddRef(m_manager);
	}

	SoundSource::~SoundSource()
	{
		auto s = (Sound*) m_manager;
		s->SoundSourcesContainer->Unregister(this);

		SafeRelease(m_sound);
		SafeRelease(m_manager);
	}

	float SoundSource::GetLoopStartingPoint() const
	{
		return m_sound->GetLoopStartingPoint();
	}

	void SoundSource::SetLoopStartingPoint(float startingPoint)
	{
		m_sound->SetLoopStartingPoint(startingPoint);
	}

	float SoundSource::GetLoopEndPoint() const
	{
		return m_sound->GetLoopEndPoint();
	}

	void SoundSource::SetLoopEndPoint(float endPoint)
	{
		m_sound->SetLoopEndPoint(endPoint);
	}

	bool SoundSource::GetIsLoopingMode() const
	{ 
		return m_sound->GetIsLoopingMode();
	}

	void SoundSource::SetIsLoopingMode(bool isLoopingMode) 
	{ 
		m_sound->SetIsLoopingMode(isLoopingMode); 
	}

	float SoundSource::GetLength()
	{
		return m_sound->GetLength();
	}

	void SoundSource::Reload(uint8_t* data, int32_t size)
	{
		auto ls = GetLoopStartingPoint();
		auto le = GetLoopEndPoint();
		auto lm = GetIsLoopingMode();

		auto s = (Sound*) m_manager;
		auto source = s->GetManager()->CreateSound(data, size, m_isDecompressed);
		if (source == nullptr) return;

		SafeRelease(m_sound);
		m_sound = source;

		SetLoopStartingPoint(ls);
		SetLoopEndPoint(le);
		SetIsLoopingMode(lm);
	}
}