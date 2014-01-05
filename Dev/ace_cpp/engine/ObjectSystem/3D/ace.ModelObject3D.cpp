
#include "ace.ModelObject3D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	ModelObject3D::ModelObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateModelObject3D());
		m_commonObject = m_coreObject.get();
	}

	ModelObject3D::~ModelObject3D()
	{

	}


	void ModelObject3D::AddMesh(std::shared_ptr<Mesh>& mesh)
	{
		m_coreObject->AddMesh(mesh.get());
	}
}