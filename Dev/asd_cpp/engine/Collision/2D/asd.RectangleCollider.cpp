#include "asd.RectangleCollider.h"

namespace asd {

	extern ObjectSystemFactory* g_objectSystemFactory;
	RectangleCollider::RectangleCollider() {
		coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateRectangleCollider());
	}

	RectangleCollider::~RectangleCollider() {
	}

	RectF RectangleCollider::GetArea() const {
		return coreObject->GetArea();
	}

	void RectangleCollider::SetArea(RectF area) {
		coreObject->SetArea(area);
	}

	std::shared_ptr<CoreCollider2D> RectangleCollider::GetCoreCollider() const {
		return coreObject;
	}

	Collider2DShapeType RectangleCollider::GetType() const {
		return Collider2DShapeType::Rectangle;
	}
}