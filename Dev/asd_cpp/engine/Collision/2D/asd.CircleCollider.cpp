#include "asd.CircleCollider.h"

namespace asd {

	extern ObjectSystemFactory* g_objectSystemFactory;

	CircleCollider::CircleCollider() {
		coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateCircleCollider());
	}

	CircleCollider::~CircleCollider() {

	}

	Vector2DF CircleCollider::GetCenter() const {
		return coreObject->GetCenter();
	}

	void CircleCollider::SetCenter(Vector2DF position) {
		coreObject->SetCenter(position);
	}

	float CircleCollider::GetRadius() const {
		return coreObject->GetRadius();
	}

	void CircleCollider::SetRadius(float radius) {
		coreObject->SetRadius(radius);
	}

	Collider2DShapeType CircleCollider::GetType() const {
		return Collider2DShapeType::Circle;
	}

	std::shared_ptr<CoreCollider2D> CircleCollider::GetCoreCollider() const {
		return coreObject;
	}
}