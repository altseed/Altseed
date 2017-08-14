#include "asd.Collider2D.h"

namespace asd {

	std::weak_ptr<Object2D> Collider2D::GetOwnerObject() const {
		return ownerObject2D;
	}

	const achar* Collider2D::GetTag() const {
		return GetCoreCollider()->GetTag();
	}

	void Collider2D::SetTag(const achar* tag) {
		GetCoreCollider()->SetTag(tag);
	}

	void Collider2D::SetVisible(bool isVisible) {
		GetCoreCollider()->SetIsVisible(isVisible);
	}

	bool Collider2D::GetVisible() {
		return GetCoreCollider()->GetIsVisible();
	}
}