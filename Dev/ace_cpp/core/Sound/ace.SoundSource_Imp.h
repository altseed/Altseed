
#pragma once

#include "ace.SoundSource.h"
#include "../ace.ReferenceObject.h"

#include <OpenSoundMixer.h>

namespace ace
{
	class SoundSource_Imp
		: public SoundSource
		, public ReferenceObject
	{
	private:
		Sound*			m_manager;
		osm::Sound*		m_sound;
	public:

		SoundSource_Imp(Sound* manager, osm::Sound* sound);
		virtual ~SoundSource_Imp();

		void SetLoopPoint(float loopStart, float loopEnd) override;

		float GetLength() override;

		osm::Sound* GetSound() { return m_sound; }

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}