
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

	void ModelObject3D::SetModel(std::shared_ptr<Model>& model)
	{
		m_coreObject->SetModel(model.get());
	}

	void ModelObject3D::AddMeshGroup()
	{
		m_coreObject->AddMeshGroup();
	}

	int32_t ModelObject3D::GetMeshGroupCount()
	{
		return m_coreObject->GetMeshGroupCount();
	}

	void ModelObject3D::AddMesh(int32_t meshGroupIndex, std::shared_ptr<Mesh> mesh)
	{
		m_coreObject->AddMesh(meshGroupIndex, mesh.get());
	}

	void ModelObject3D::SetDeformer(int32_t meshGroupIndex, std::shared_ptr<Deformer> deformer)
	{
		m_coreObject->SetDeformer(meshGroupIndex, deformer.get());
	}

	void ModelObject3D::PlayAnimation(const achar* name)
	{
		m_coreObject->PlayAnimation(name);
	}
}