
#include "ace.MeshObject3D.h"

namespace ace
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	MeshObject3D::MeshObject3D()
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateMeshObject3D());
		m_commonObject = m_coreObject.get();
	}

	MeshObject3D::~MeshObject3D()
	{

	}


	void MeshObject3D::AddMesh(std::shared_ptr<Mesh>& mesh)
	{
		m_coreObject->AddMesh(mesh.get());
	}
}