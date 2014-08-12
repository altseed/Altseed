
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
		bool			isDecompressed = false;

	public:

		SoundSource_Imp(Sound* manager, osm::Sound* sound, bool isDecomporessed);
		virtual ~SoundSource_Imp();

		float GetLoopStartingPoint() const override { return m_sound->GetLoopStartingPoint(); }

		void SetLoopStartingPoint(float startingPoint) override { m_sound->SetLoopStartingPoint(startingPoint); }

		float GetLoopEndPoint() const override { return m_sound->GetLoopEndPoint(); }

		void SetLoopEndPoint(float endPoint) override { m_sound->SetLoopEndPoint(endPoint); }

		bool GetIsLoopingMode() const override { return m_sound->GetIsLoopingMode(); }

		void SetIsLoopingMode(bool isLoopingMode) override { m_sound->SetIsLoopingMode(isLoopingMode); }

		float GetLength() override;

		osm::Sound* GetSound() { return m_sound; }

		void Reload(uint8_t* data, int32_t size);

		// IReferenceを継承したデバイスオブジェクト向け定義
#if !SWIG
	public:
		virtual int GetRef() { return ReferenceObject::GetRef(); }
		virtual int AddRef() { return ReferenceObject::AddRef(); }
		virtual int Release() { return ReferenceObject::Release(); }
#endif
	};
}