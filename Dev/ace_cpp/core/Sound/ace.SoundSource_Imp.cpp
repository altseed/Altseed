
#include "ace.Sound_Imp.h"
#include "ace.SoundSource_Imp.h"

namespace ace
{
	SoundSource_Imp::SoundSource_Imp(Sound* manager, osm::Sound* sound)
	{
		m_manager = manager;
		m_sound = sound;

		SafeAddRef(m_manager);
		SafeAddRef(m_sound);
	}

	SoundSource_Imp::~SoundSource_Imp()
	{
		SafeRelease(m_sound);
		SafeRelease(m_manager);
	}

	void SoundSource_Imp::SetLoopPoint(float loopStart, float loopEnd)
	{
		m_sound->SetLoopPoint(loopStart, loopEnd);
	}

	float SoundSource_Imp::GetLength()
	{
		return m_sound->GetLength();
	}
}