#include <memory>
#include "asd.TransformInfo2D.h"
#include "../common/Math/asd.Matrix33.h"
using namespace std;

namespace asd
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

	ChildTransformingMode TransformInfo2D::GetChildTransformingMode() const
	{
		return m_parentInfo->GetChildTransformingMode();
	}

	void TransformInfo2D::SetChildTransformingMode(ChildTransformingMode value)
	{
		m_parentInfo->SetChildTransformingMode(value);
	}

}