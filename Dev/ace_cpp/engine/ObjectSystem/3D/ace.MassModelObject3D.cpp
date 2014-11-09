#include "ace.MassModelObject3D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	MassModelObject3D::MassModelObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCoreMassModelObject3D());
		m_commonObject = m_coreObject.get();
	}

	MassModelObject3D::~MassModelObject3D()
	{

	}

	void MassModelObject3D::SetModel(std::shared_ptr<MassModel>& model)
	{
		m_coreObject->SetModel(model.get());
	}

	void MassModelObject3D::PlayAnimation(const achar* name)
	{
		m_coreObject->PlayAnimation(name);
	}

	void MassModelObject3D::StopAnimation()
	{
		m_coreObject->StopAnimation();
	}

	void MassModelObject3D::CrossFadeAnimation(const achar* name, float time)
	{
		m_coreObject->CrossFadeAnimation(name, time);
	}

	bool MassModelObject3D::IsAnimationPlaying()
	{
		return m_coreObject->IsAnimationPlaying();
	}

	void MassModelObject3D::SetMaterialPropertyBlock(std::shared_ptr<MaterialPropertyBlock> block)
	{
		m_coreObject->SetMaterialPropertyBlock(block.get());
	}
}