
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

	void ModelObject3D::AddMesh(std::shared_ptr<Mesh> mesh)
	{
		m_coreObject->AddMesh(mesh.get());
	}

	void ModelObject3D::SetDeformer(std::shared_ptr<Deformer> deformer)
	{
		m_coreObject->SetDeformer(deformer.get());
	}

	void ModelObject3D::PlayAnimation(const achar* name)
	{
		m_coreObject->PlayAnimation(name);
	}

	void ModelObject3D::SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, std::shared_ptr<MaterialPropertyBlock> block)
	{
		m_coreObject->SetMaterialPropertyBlock(meshIndex, materialIndex, block.get());
	}
}