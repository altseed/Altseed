
#include "ace.CoreObject3D_Imp.h"
#include "ace.CoreLayer3D_Imp.h"

#include "../Core/Graphics/Common/3D/ace.RenderedObject3D.h"

namespace ace
{
	CoreObject3D_Imp::CoreObject3D_Imp()
		: m_commonObject(nullptr)
		, m_isDrawn(true)
	{
	}

	CoreObject3D_Imp::~CoreObject3D_Imp()
	{
	}

	bool CoreObject3D_Imp::GetIsDrawn() const
	{
		return m_isDrawn;
	}

	void CoreObject3D_Imp::SetIsDrawn(bool value)
	{
		m_isDrawn = value;
	}

	Vector3DF CoreObject3D_Imp::GetPosition() const
	{
		return m_commonObject->GetPosition();
	}

	void CoreObject3D_Imp::SetPosition(Vector3DF position)
	{
		m_commonObject->SetPosition(position);
	}

	void CoreObject3D_Imp::SetLayer(CoreLayer3D* layer)
	{
		if (layer != nullptr)
		{
			((CoreLayer3D_Imp*) layer)->GetRenderer()->AddObject(m_commonObject);
		}
		else
		{
			((CoreLayer3D_Imp*) layer)->GetRenderer()->RemoveObject(m_commonObject);
		}
	}
}
