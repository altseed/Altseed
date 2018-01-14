#include "asd.PolygonCollider.h"

namespace asd {

	extern ObjectSystemFactory* g_objectSystemFactory;

	PolygonCollider::PolygonCollider() {
		coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreatePolygonCollider());
	}

	PolygonCollider::~PolygonCollider() {

	}

	bool PolygonCollider::GetIsValid() {
		return coreObject->GetIsValid();
	}

	void PolygonCollider::AddVertex(Vector2DF vertex) {
		coreObject->AddVertex(vertex);
	}

	void PolygonCollider::ClearVertexes() {
		coreObject->ClearVertexes();
	}

	int PolygonCollider::GetVertexesNum() const {
		return coreObject->GetVertexesNum();
	}

	void PolygonCollider::SetVertex(int index, Vector2DF vertex) {
		coreObject->SetVertex(index, vertex);
	}

	Vector2DF PolygonCollider::GetVertex(int index) const {
		return coreObject->GetVertex(index);
	}

	std::shared_ptr<CoreCollider2D> PolygonCollider::GetCoreCollider() const {
		return coreObject;
	}

	Collider2DShapeType PolygonCollider::GetShapeType() const {
		return Collider2DShapeType::Polygon;
	}
}