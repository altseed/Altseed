
#include "ace.CoreModelObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/ace.RenderedModelObject3D.h"

namespace ace
{
	CoreModelObject3D_Imp::CoreModelObject3D_Imp(Graphics* graphics)
		: m_object(nullptr)
	{
		m_object = new RenderedModelObject3D(graphics);
		m_commonObject = m_object;
	}

	CoreModelObject3D_Imp::~CoreModelObject3D_Imp()
	{
		SafeRelease(m_object);
	}

	void CoreModelObject3D_Imp::SetModel(Model* model)
	{
		m_object->SetModel(model);
	}

	void CoreModelObject3D_Imp::AddMesh(Mesh* mesh)
	{
		m_object->AddMesh(mesh);
	}

	void CoreModelObject3D_Imp::SetDeformer(Deformer* deformer)
	{
		m_object->SetDeformer(deformer);
	}

	AnimationClip* CoreModelObject3D_Imp::GetAnimationClip(const achar* name)
	{
		return m_object->GetAnimationClip(name);
	}

	void CoreModelObject3D_Imp::PlayAnimation(int32_t index, const achar* name)
	{
		m_object->PlayAnimation(index, name);
	}

	void CoreModelObject3D_Imp::StopAnimation(int32_t index)
	{
		m_object->StopAnimation(index);
	}

	void CoreModelObject3D_Imp::SetAnimationWeight(int32_t index, float weight)
	{
		m_object->SetAnimationWeight(index, weight);
	}

	void CoreModelObject3D_Imp::CrossFadeAnimation(int32_t index, const achar* name, float time)
	{
		m_object->CrossFadeAnimation(index, name, time);
	}

	bool CoreModelObject3D_Imp::IsAnimationPlaying(int32_t index)
	{
		return m_object->IsAnimationPlaying(index);
	}

	void CoreModelObject3D_Imp::SetMaterialPropertyBlock(int32_t meshIndex, int32_t materialIndex, MaterialPropertyBlock* block)
	{
		m_object->SetMaterialPropertyBlock(meshIndex, materialIndex, block);
	}
}
