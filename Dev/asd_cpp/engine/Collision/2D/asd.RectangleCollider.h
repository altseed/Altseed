#pragma once
#include "asd.Collider2D.h"

namespace asd {

	class RectangleCollider : public Collider2D{
	private:
		std::shared_ptr<CoreRectangleCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;
	public:
		RectangleCollider();
		virtual ~RectangleCollider();

		RectF GetArea() const;
		void SetArea(RectF area);

		Collider2DShapeType GetType() const override;
	};
}