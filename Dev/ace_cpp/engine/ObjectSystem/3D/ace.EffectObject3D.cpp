
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
}