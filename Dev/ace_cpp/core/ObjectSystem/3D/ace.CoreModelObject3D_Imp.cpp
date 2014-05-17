
#include "ace.CoreModelObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/ace.RenderedModelObject3D.h"

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

	void CoreModelObject3D_Imp::AddMeshGroup()
	{
		m_object->AddMeshGroup();
	}

	int32_t CoreModelObject3D_Imp::GetMeshGroupCount()
	{
		return m_object->GetMeshGroupCount();
	}

	void CoreModelObject3D_Imp::AddMesh(int32_t meshGroupIndex, Mesh* mesh)
	{
		m_object->AddMesh(meshGroupIndex, mesh);
	}

	void CoreModelObject3D_Imp::SetDeformer(int32_t meshGroupIndex, Deformer* deformer)
	{
		m_object->SetDeformer(meshGroupIndex, deformer);
	}

	void CoreModelObject3D_Imp::PlayAnimation(const achar* name)
	{
		m_object->PlayAnimation(name);
	}
}
