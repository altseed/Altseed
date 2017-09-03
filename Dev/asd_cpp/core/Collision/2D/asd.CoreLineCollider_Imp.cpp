#include "asd.CoreLineCollider_Imp.h"

namespace asd {

	CoreLineCollider_Imp::CoreLineCollider_Imp() :
		startingPosition(Vector2DF(0, 0)),
		endingPosition(Vector2DF(0, 0)),
		thickness(0),
		isVisible(false)
	{

	}

	CoreLineCollider_Imp::~CoreLineCollider_Imp() {
	}

	Vector2DF CoreLineCollider_Imp::GetStartingPosition() const {
		return startingPosition;
	}

	void CoreLineCollider_Imp::SetStartingPosition(Vector2DF startingPosition) {
		this->startingPosition = startingPosition;
	}

	Vector2DF CoreLineCollider_Imp::GetEndingPosition() const {
		return endingPosition;
	}

	void CoreLineCollider_Imp::SetEndingPosition(Vector2DF endingPosition) {
		this->endingPosition = endingPosition;
	}

	float CoreLineCollider_Imp::GetThickness() const {
		return thickness;
	}

	void CoreLineCollider_Imp::SetThickness(float thickness) {
		this->thickness = thickness;
	}

	Collider2DShapeType CoreLineCollider_Imp::GetShapeType() const {
		return Collider2DShapeType::Line;
	}

	void CoreLineCollider_Imp::Update() {

		if (ownerObject == nullptr) {
			return;
		}
		Vector2DF vector = endingPosition - startingPosition;

		auto binorm = vector;
		{
			auto deg = binorm.GetDegree();
			deg += 90;
			binorm.SetDegree(deg);
			binorm.Normalize();
		}

		auto halfThickness = thickness / 2;

		std::array<Vector2DF, 4> vertexes = { startingPosition + binorm*halfThickness, endingPosition + binorm*halfThickness, endingPosition - binorm*halfThickness, startingPosition - binorm*halfThickness };

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

	b2Shape* CoreLineCollider_Imp::GetB2Shape() {
		return &b2polygonShape;
	}

	const achar* CoreLineCollider_Imp::GetTag() const {
		return tag.c_str();
	}

	void CoreLineCollider_Imp::SetTag(const achar* tag) {
		this->tag = astring(tag);
	}

	bool CoreLineCollider_Imp::GetIsVisible() const {
		return isVisible;
	}

	void CoreLineCollider_Imp::SetIsVisible(bool isVisible) {
		this->isVisible = isVisible;
	}

	void CoreLineCollider_Imp::DrawVisibleCollisionsAdditionally(CoreLayer2D* layer) {
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

	int64_t CoreLineCollider_Imp::GetCreationId() const {
		return creationId;
	}
}