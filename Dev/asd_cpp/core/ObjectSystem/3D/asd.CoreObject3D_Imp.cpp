
#include "asd.CoreObject3D_Imp.h"
#include "asd.CoreLayer3D_Imp.h"

#include "../../Graphics/3D/Object/asd.RenderedObject3D.h"

namespace asd
{
	CoreObject3D_Imp::CoreObject3D_Imp()
		: m_commonObject(nullptr)
		, m_isDrawn(true)
		, m_owner(nullptr)
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

	Vector3DF CoreObject3D_Imp::GetRotation() const
	{
		return m_commonObject->GetRotation();
	}

	void CoreObject3D_Imp::SetRotation(const Vector3DF& rot)
	{
		m_commonObject->SetRotation(rot);
	}

	Vector3DF CoreObject3D_Imp::GetScale() const
	{
		return m_commonObject->GetScale();
	}

	void CoreObject3D_Imp::SetScale(const Vector3DF& scale)
	{
		m_commonObject->SetScale(scale);
	}

	Matrix44 CoreObject3D_Imp::GetLocalMatrix()
	{
		return m_commonObject->GetLocalMatrix();
	}

	void CoreObject3D_Imp::SetLoalMatrixDirectly(Matrix44 localMat)
	{
		m_commonObject->SetLoalMatrixDirectly(localMat);
	}

	void CoreObject3D_Imp::SetLayer(CoreLayer3D* layer)
	{
		if (layer != nullptr)
		{
			m_owner = layer;
			((CoreLayer3D_Imp*) layer)->GetRenderer()->AddObject(m_commonObject);
		}
		else
		{
			((CoreLayer3D_Imp*) m_owner)->GetRenderer()->RemoveObject(m_commonObject);
			m_owner = nullptr;
		}
	}
}
