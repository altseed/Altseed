#pragma once
#include <memory>
#include "ace.ICoreObject2D.h"
#include "ace.ParentInfo2D.h"

namespace ace
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
		Vector2DF GetGlobalPosition();

		float GetAngle() const;
		void SetAngle(float value);

		Vector2DF GetScale() const;
		void SetScale(Vector2DF value);

		ICoreObject2D& GetParent();
		void SetParent(ICoreObject2D& parent, eChildMode mode);
		void ClearParent();

		eChildMode GetChildMode() const;
		void SetChildMode(eChildMode value);

		Matrix33 GetMatrixToTranslate();
		Matrix33 GetMatrixToTransform();

		Matrix33 GetParentsMatrix();
	};
}