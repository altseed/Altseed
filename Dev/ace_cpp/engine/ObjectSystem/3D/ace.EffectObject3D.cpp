
#include "ace.EffectObject3D.h"

namespace ace
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

	void EffectObject3D::Play()
	{
		m_coreObject->Play();
	}

	void EffectObject3D::Stop()
	{
		m_coreObject->Stop();
	}

	void EffectObject3D::StopRoot()
	{
		m_coreObject->StopRoot();
	}

	bool EffectObject3D::GetDoesMoveEffects()
	{
		return m_coreObject->GetDoesMoveEffects();
	}

	void EffectObject3D::SetDoesMoveEffects(bool value)
	{
		m_coreObject->SetDoesMoveEffects(value);
	}
}