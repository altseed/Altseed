#include <memory>
#include "ace.TransformInfo2D.h"
#include "../common/Math/ace.Matrix33.h"
using namespace std;

namespace ace
{
	TransformInfo2D::TransformInfo2D()
		: m_position(Vector2DF())
		, m_angle(0)
		, m_scale(Vector2DF(1, 1))
		, m_parentInfo(nullptr)
		, m_matrixToTranslate(nullptr)
		, m_matrixToTransform(nullptr)
	{
	}

	TransformInfo2D::~TransformInfo2D()
	{
	}


	Vector2DF TransformInfo2D::GetPosition() const
	{
		return m_position;
	}

	void TransformInfo2D::SetPosition(Vector2DF value)
	{
		m_position = value;
		m_matrixToTranslate = nullptr;
		m_matrixToTransform = nullptr;
	}

	Vector2DF TransformInfo2D::GetGlobalPosition()
	{
		auto pos = Vector3DF(m_position.X, m_position.Y, 1);
		Vector3DF result = GetParentsMatrix() * pos;
		return Vector2DF(result.X, result.Y);
	}

	float TransformInfo2D::GetAngle() const
	{
		return m_angle;
	}

	void TransformInfo2D::SetAngle(float value)
	{
		m_angle = value;
		m_matrixToTransform = nullptr;
	}

	Vector2DF TransformInfo2D::GetScale() const
	{
		return m_scale;
	}

	void TransformInfo2D::SetScale(Vector2DF value)
	{
		m_scale = value;
		m_matrixToTransform = nullptr;
	}


	Matrix33 TransformInfo2D::GetMatrixToTranslate()
	{
		if (m_matrixToTranslate == nullptr)
		{
			m_matrixToTranslate = make_shared<Matrix33>();
			m_matrixToTranslate->SetTranslation(m_position.X, m_position.Y);
		}
		return *m_matrixToTranslate;
	}

	Matrix33 TransformInfo2D::GetMatrixToTransform()
	{
		if (m_matrixToTransform == nullptr)
		{
			auto translation = Matrix33().SetTranslation(m_position.X, m_position.Y);
			auto rotation = Matrix33().SetRotation(DegreeToRadian(m_angle));
			auto scale = Matrix33().SetScale(m_scale.X, m_scale.Y);
			m_matrixToTransform = make_shared<Matrix33>();
			*m_matrixToTransform = translation * rotation * scale;
		}
		return *m_matrixToTransform;
	}

	Matrix33 TransformInfo2D::GetParentsMatrix()
	{
		if (m_parentInfo != nullptr)
		{
			auto& parent = m_parentInfo->GetParent();
			switch (m_parentInfo->GetChildMode())
			{
			case eChildMode::CHILD_MODE_POSITION:
				return parent.GetParentsMatrix() * parent.GetMatrixToTranslate();

			case eChildMode::CHILD_MODE_ALL:
				return parent.GetParentsMatrix() * parent.GetMatrixToTransform();

			case eChildMode::CHILD_MODE_NOTHING:
				return parent.GetParentsMatrix() * Matrix33();
			}
			assert(false);
		}
		else
		{
			return Matrix33();
		}
	}


	CoreObject2D& TransformInfo2D::GetParent()
	{
		return m_parentInfo->GetParent();
	}

	void TransformInfo2D::SetParent(CoreObject2D& parent, eChildMode mode)
	{
		if (m_parentInfo != nullptr)
		{
			throw "子オブジェクトとして追加しようとしたオブジェクトは、すでに別のオブジェクトの子として追加されています。";
		}
		m_parentInfo = make_shared<ParentInfo2D>(&parent, mode);
	}

	void TransformInfo2D::ClearParent()
	{
		m_parentInfo = nullptr;
	}

	eChildMode TransformInfo2D::GetChildMode() const
	{
		return m_parentInfo->GetChildMode();
	}

	void TransformInfo2D::SetChildMode(eChildMode value)
	{
		m_parentInfo->SetChildMode(value);
	}

}