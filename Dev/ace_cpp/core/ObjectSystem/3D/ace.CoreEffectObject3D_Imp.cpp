
#include "ace.CoreEffectObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/Common/3D/ace.RenderedEffectObject3D.h"

namespace ace
{
	CoreEffectObject3D_Imp::CoreEffectObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedEffectObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreEffectObject3D_Imp::~CoreEffectObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreEffectObject3D_Imp::SetEffect(Effect* effect)
	{
		m_object->SetEffect(effect);
	}

	void CoreEffectObject3D_Imp::Play()
	{
		m_object->Play();
	}
}
