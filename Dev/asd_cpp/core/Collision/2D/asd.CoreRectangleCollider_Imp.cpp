#include "asd.CoreRectangleCollider_Imp.h"
#include <Box2D/Box2D.h>

namespace asd {

	CoreRectangleCollider_Imp::CoreRectangleCollider_Imp() :
		area(RectF(0, 0, 0, 0)),
		isVisible(false)
	{

	}

	RectF CoreRectangleCollider_Imp::GetArea() const {
		return area;
	}

	void CoreRectangleCollider_Imp::SetArea(RectF area) {
		this->area = area;
	}

	Collider2DShapeType CoreRectangleCollider_Imp::GetShapeType() const {
		return Collider2DShapeType::Rectangle;
	}

	void CoreRectangleCollider_Imp::Update() {

		if (ownerObject == nullptr) {
			return;
		}

		auto vertexes = area.GetVertexes();

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

	b2Shape* CoreRectangleCollider_Imp::GetB2Shape() {
		return &b2polygonShape;
	}

	const achar* CoreRectangleCollider_Imp::GetTag() const {
		return tag.c_str();
	}

	void CoreRectangleCollider_Imp::SetTag(const achar* tag) {
		this->tag = astring(tag);
	}

	bool CoreRectangleCollider_Imp::GetIsVisible() const {
		return isVisible;
	}

	void CoreRectangleCollider_Imp::SetIsVisible(bool isVisible) {
		this->isVisible = isVisible;
	}

	void CoreRectangleCollider_Imp::DrawVisibleCollisionsAdditionally(CoreLayer2D* layer) {
		auto vertices = b2polygonShape.m_vertices;
		layer->DrawTriangleAdditionally(
			Vector2DF(vertices[0].x, vertices[0].y),
			Vector2DF(vertices[1].x, vertices[1].y),
			Vector2DF(vertices[2].x, vertices[2].y),
			Color(255, 0, 0, 100),
			Vector2DF(),
			Vector2DF(),
			Vector2DF(),
			nullptr,
			AlphaBlendMode::Blend,
			10000);

		layer->DrawTriangleAdditionally(
			Vector2DF(vertices[2].x, vertices[2].y),
			Vector2DF(vertices[3].x, vertices[3].y),
			Vector2DF(vertices[0].x, vertices[0].y),
			Color(255, 0, 0, 100),
			Vector2DF(),
			Vector2DF(),
			Vector2DF(),
			nullptr,
			AlphaBlendMode::Blend,
			10000);
	}

	int64_t CoreRectangleCollider_Imp::GetCreationId() const {
		return creationId;
	}
}