
#include "ace.CoreEffectObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/ace.RenderedEffectObject3D.h"

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

	void CoreEffectObject3D_Imp::Stop()
	{
		m_object->Stop();
	}

	void CoreEffectObject3D_Imp::StopRoot()
	{
		m_object->StopRoot();
	}

	bool CoreEffectObject3D_Imp::GetSyncEffects()
	{
		return m_object->GetSyncEffects();
	}

	void CoreEffectObject3D_Imp::SetSyncEffects(bool value)
	{
		m_object->SetSyncEffects(value);
	}
}
