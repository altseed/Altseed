#include "asd.CorePolygonCollider_Imp.h"
#include <Box2D/Box2D.h>

namespace asd {

	CorePolygonCollider_Imp::CorePolygonCollider_Imp()
		:isVisible(false)
	{

	}

	CorePolygonCollider_Imp::~CorePolygonCollider_Imp() {

	}

	bool CorePolygonCollider_Imp::IsValid() {
		return false;
	}

	void CorePolygonCollider_Imp::AddVertex(Vector2DF vertex) {
		vertexes.push_back(vertex);
	}

	void CorePolygonCollider_Imp::ClearVertexes() {
		vertexes.clear();
	}

	int CorePolygonCollider_Imp::GetVertexesNum() const {
		return vertexes.size();
	}

	void CorePolygonCollider_Imp::SetVertex(int index, Vector2DF vertex) {
		vertexes[index] = vertex;
	}

	Vector2DF CorePolygonCollider_Imp::GetVertex(int index) const {
		return vertexes[index];
	}

	Collider2DShapeType CorePolygonCollider_Imp::GetShapeType() const {
		return Collider2DShapeType::Polygon;
	}

	void CorePolygonCollider_Imp::Update() {

		if (ownerObject == nullptr) {
			return;
		}

		std::vector<b2Vec2> polyPoints;
		culling2d::Vector2DF minPos = culling2d::Vector2DF(FLT_MAX, FLT_MAX);
		culling2d::Vector2DF maxPos = culling2d::Vector2DF(FLT_MIN, FLT_MIN);

		auto matrix = ownerObject->GetAbsoluteMatrixToTransform();
		for (auto vertex : vertexes)
		{

			auto vc = Vector3DF(vertex.X, vertex.Y, 1);
			auto res1 = matrix * vc;
			auto pos = Vector2DF(res1.X, res1.Y);

			polyPoints.push_back(b2Vec2(pos.X, pos.Y));
			minPos.X = Min(minPos.X, pos.X);
			minPos.Y = Min(minPos.Y, pos.Y);
			maxPos.X = Max(maxPos.X, pos.X);
			maxPos.Y = Max(maxPos.Y, pos.Y);
		}
		aabb.X = minPos.X;
		aabb.Y = minPos.Y;
		aabb.Width = maxPos.X - minPos.X;
		aabb.Height = maxPos.Y - minPos.Y;

		b2polygonShape.Set(polyPoints.data(), polyPoints.size());

		coreCollision2DManager->NotifyLastTransformed(this);
	}

	b2Shape* CorePolygonCollider_Imp::GetB2Shape() {
		return &b2polygonShape;
	}

	const achar* CorePolygonCollider_Imp::GetTag() const {
		return tag.c_str();
	}

	void CorePolygonCollider_Imp::SetTag(const achar* tag) {
		this->tag = astring(tag);
	}

	bool CorePolygonCollider_Imp::GetIsVisible() const {
		return isVisible;
	}

	void CorePolygonCollider_Imp::SetIsVisible(bool isVisible) {
		this->isVisible = isVisible;
	}

	void CorePolygonCollider_Imp::DrawVisibleCollisionsAdditionally(CoreLayer2D* layer) {
		//TODO: 書く
	}

	int64_t CorePolygonCollider_Imp::GetCreationId() const {
		return creationId;
	}
}