
#include "asd.CoreEffectObject3D_Imp.h"
#include "asd.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/asd.RenderedEffectObject3D.h"

namespace asd
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

	bool CoreEffectObject3D_Imp::GetIsPlaying()
	{
		return m_object->GetIsPlaying();
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
