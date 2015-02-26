#include "ace.EffectObject2D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	EffectObject2D::EffectObject2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateEffectObject2D());
	}

	EffectObject2D::~EffectObject2D()
	{
	}

	CoreObject2D* EffectObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}

	std::shared_ptr<Effect> EffectObject2D::GetEffect()
	{
		return m_coreObject->GetEffect();
	}

	void EffectObject2D::SetEffect(std::shared_ptr<Effect> effect)
	{
		m_coreObject->SetEffect(effect.get());
	}

	void EffectObject2D::Play()
	{
		m_coreObject->Play();
	}

	void EffectObject2D::Stop()
	{
		m_coreObject->Stop();
	}

	void EffectObject2D::StopRoot()
	{
		m_coreObject->StopRoot();
	}

	bool EffectObject2D::GetIsPlaying()
	{
		return m_coreObject->GetIsPlaying();
	}

	bool EffectObject2D::GetSyncEffects()
	{
		return m_coreObject->GetSyncEffects();
	}

	void EffectObject2D::SetSyncEffects(bool value)
	{
		m_coreObject->SetSyncEffects(value);
	}

	float EffectObject2D::GetEffectRotation() const
	{
		return m_coreObject->GetEffectRotation();
	}

	void EffectObject2D::SetEffectRotation(float value)
	{
		m_coreObject->SetEffectRotation(value);
	}

	int EffectObject2D::GetDrawingPriority() const
	{
		return m_coreObject->GetDrawingPriority();
	}

	void EffectObject2D::SetDrawingPriority(int priority)
	{
		m_coreObject->SetDrawingPriority(priority);
	}
}