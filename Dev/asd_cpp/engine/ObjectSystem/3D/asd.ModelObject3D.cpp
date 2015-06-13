
#include "asd.ModelObject3D.h"

namespace asd
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

	std::shared_ptr<AnimationClip> ModelObject3D::GetAnimationClip(const achar* name)
	{
		auto anim = m_coreObject->GetAnimationClip(name);
		if (anim == nullptr) return nullptr;
		SafeAddRef(anim);
		return CreateSharedPtrWithReleaseDLL(anim);
	}

	void ModelObject3D::PlayAnimation(int32_t index, const achar* name)
	{
		m_coreObject->PlayAnimation(index, name);
	}

	void ModelObject3D::StopAnimation(int32_t index)
	{
		m_coreObject->StopAnimation(index);
	}

	void ModelObject3D::SetAnimationWeight(int32_t index, float weight)
	{
		m_coreObject->SetAnimationWeight(index, weight);
	}

	void ModelObject3D::CrossFadeAnimation(int32_t index, const achar* name, float time)
	{
		m_coreObject->CrossFadeAnimation(index, name, time);
	}

	bool ModelObject3D::GetIsAnimationPlaying(int32_t index)
	{
		return m_coreObject->GetIsAnimationPlaying(index);
	}

	void ModelObject3D::SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, std::shared_ptr<MaterialPropertyBlock> block)
	{
		m_coreObject->SetMaterialPropertyBlock(meshIndex, materialIndex, block.get());
	}
}