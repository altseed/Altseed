
#include "asd.EffectObject3D.h"

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	EffectObject3D::EffectObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateEffectObject3D());
		m_commonObject = m_coreObject.get();
	}

	EffectObject3D::~EffectObject3D()
	{

	}

	void EffectObject3D::SetEffect(std::shared_ptr<Effect> effect)
	{
		m_coreObject->SetEffect(effect.get());
	}

	int32_t EffectObject3D::Play()
	{
		return m_coreObject->Play();
	}

	void EffectObject3D::Stop()
	{
		m_coreObject->Stop();
	}

	void EffectObject3D::StopRoot()
	{
		m_coreObject->StopRoot();
	}

	bool EffectObject3D::GetIsPlaying()
	{
		return m_coreObject->GetIsPlaying();
	}

	bool EffectObject3D::GetSyncEffects()
	{
		return m_coreObject->GetSyncEffects();
	}

	void EffectObject3D::SetSyncEffects(bool value)
	{
		m_coreObject->SetSyncEffects(value);
	}
}