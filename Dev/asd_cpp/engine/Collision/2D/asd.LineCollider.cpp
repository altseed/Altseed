#include "asd.LineCollider.h"

namespace asd {

	extern ObjectSystemFactory* g_objectSystemFactory;

	LineCollider::LineCollider() {
		coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateLineCollider());
	}

	LineCollider::~LineCollider() {

	}

	Vector2DF LineCollider::GetStartingPosition() const {
		return coreObject->GetStartingPosition();
	}

	void LineCollider::SetStartingPosition(Vector2DF startingPosition) {
		coreObject->SetStartingPosition(startingPosition);
	}

	Vector2DF LineCollider::GetEndingPosition() const {
		return coreObject->GetEndingPosition();
	}

	void LineCollider::SetEndingPosition(Vector2DF endingPosition) {
		coreObject->SetEndingPosition(endingPosition);
	}

	float LineCollider::GetThickness() const {
		return coreObject->GetThickness();
	}

	void LineCollider::SetThickness(float thickness) {
		coreObject->SetThickness(thickness);
	}

	Collider2DShapeType LineCollider::GetType() const {
		return Collider2DShapeType::Line;
	}

	std::shared_ptr<CoreCollider2D> LineCollider::GetCoreCollider() const {
		return coreObject;
	}
}