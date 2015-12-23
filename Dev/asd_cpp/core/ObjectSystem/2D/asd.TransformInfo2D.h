#pragma once
#include <memory>
#include "asd.CoreObject2D.h"
#include "asd.ParentInfo2D.h"
#include "asd.CoreLayer2D.h"

namespace asd
{
	class TransformInfo2D
	{
	private:
		std::shared_ptr<ParentInfo2D> m_parentInfo;
		std::shared_ptr<Matrix33> m_matrixToTranslate;
		std::shared_ptr<Matrix33> m_matrixToTransform;

		Vector2DF m_position;
		float m_angle;
		Vector2DF m_scale;

	public:
		TransformInfo2D();
		virtual ~TransformInfo2D();

		Vector2DF GetPosition() const;
		void SetPosition(Vector2DF position);

		float GetAngle() const;
		void SetAngle(float value);

		Vector2DF GetScale() const;
		void SetScale(Vector2DF value);

		ChildTransformingMode GetChildTransformingMode() const;
		void SetChildTransformingMode(ChildTransformingMode value);

		Matrix33 GetMatrixToTranslate();
		Matrix33 GetMatrixToTransform();
	};
}