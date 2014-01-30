
#include "ace.Effect_Imp.h"

namespace ace
{
	Effect_Imp::Effect_Imp(Effekseer::Effect* effect)
		: m_effect(effect)
	{
	}

	Effect_Imp::~Effect_Imp()
	{
		SafeRelease(m_effect);
	}

	Effect_Imp* Effect_Imp::CreateEffect(Graphics* graphics)
	{
		return nullptr;
	}
}