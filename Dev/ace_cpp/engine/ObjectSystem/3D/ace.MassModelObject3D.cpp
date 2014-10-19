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
}