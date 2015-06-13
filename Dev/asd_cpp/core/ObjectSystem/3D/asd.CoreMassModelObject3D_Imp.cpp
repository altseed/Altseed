
#include "asd.CoreMassModelObject3D_Imp.h"
#include "asd.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/asd.RenderedMassModelObject3D.h"

namespace asd
{
	CoreMassModelObject3D_Imp::CoreMassModelObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedMassModelObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreMassModelObject3D_Imp::~CoreMassModelObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreMassModelObject3D_Imp::SetMaterialPropertyBlock(MaterialPropertyBlock* block)
	{
		m_object->SetMaterialPropertyBlock(block);
	}

	void CoreMassModelObject3D_Imp::SetModel(MassModel* model)
	{
		m_object->SetModel(model);
	}

	void CoreMassModelObject3D_Imp::PlayAnimation(const achar* name)
	{
		m_object->PlayAnimation(name);
	}

	void CoreMassModelObject3D_Imp::StopAnimation()
	{
		m_object->StopAnimation();
	}

	void CoreMassModelObject3D_Imp::CrossFadeAnimation(const achar* name, float time)
	{
		m_object->CrossFadeAnimation(name, time);
	}

	bool CoreMassModelObject3D_Imp::GetIsAnimationPlaying()
	{
		return m_object->GetIsAnimationPlaying();
	}
}
