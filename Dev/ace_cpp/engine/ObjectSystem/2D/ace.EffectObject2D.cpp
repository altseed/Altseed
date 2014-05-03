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
}