
#include "asd.CoreCircleCollider_Imp.h"

namespace asd {

	CoreCircleCollider_Imp::CoreCircleCollider_Imp() :
		center(Vector2DF(0, 0)),
		radius(0),
		isVisible(false)
	{

	}

	CoreCircleCollider_Imp::~CoreCircleCollider_Imp() {
		//SafeDelete(circleShape);
	}

	Vector2DF CoreCircleCollider_Imp::GetCenter() const {
		return center;
	}

	void CoreCircleCollider_Imp::SetCenter(Vector2DF center) {
		this->center = center;

	}

	float CoreCircleCollider_Imp::GetRadius() const {
		return radius;
	}

	void CoreCircleCollider_Imp::SetRadius(float radius) {
		this->radius = radius;
	}

	Collider2DShapeType CoreCircleCollider_Imp::GetType() const {
		return Collider2DShapeType::Circle;
	}

	void CoreCircleCollider_Imp::Update() {

		if (ownerObject == nullptr) {
			return;
		}

		auto matrix = ownerObject->GetAbsoluteMatrixToTransform();

		auto vc = Vector3DF(center.X, center.Y, 1);
		auto res1 = matrix * vc;
		auto tcenter = Vector2DF(res1.X, res1.Y);

		auto vr = Vector3DF(center.X + radius, center.Y, 1);
		auto res2 = matrix * vr;
		auto tradpos = Vector2DF(res2.X, res2.Y);
		auto tradius = (tcenter - tradpos).GetLength();

		aabb.X = tcenter.X - tradius;
		aabb.Y = tcenter.Y - tradius;
		aabb.Width = aabb.Height = tradius * 2;

		b2circleShape.m_p = b2Vec2(tcenter.X, tcenter.Y);
		b2circleShape.m_radius = tradius;

		coreCollision2DManager->NotifyLastTransformed(this);
	}

	b2Shape* CoreCircleCollider_Imp::GetB2Shape() {
		return &b2circleShape;
	}

	const achar* CoreCircleCollider_Imp::GetTag() const {
		return tag.c_str();
	}

	void CoreCircleCollider_Imp::SetTag(const achar* tag) {
		this->tag = astring(tag);
	}

	bool CoreCircleCollider_Imp::GetIsVisible() const {
		return isVisible;
	}

	void CoreCircleCollider_Imp::SetIsVisible(bool isVisible) {
		this->isVisible = isVisible;
	}

	void CoreCircleCollider_Imp::DrawVisibleCollisionsAdditionally(CoreLayer2D* layer) {
		layer->DrawCircleAdditionally(
			Vector2DF(b2circleShape.m_p.x, b2circleShape.m_p.y), 
			b2circleShape.m_radius * 2, 
			0, 
			asd::Color(255, 0, 0, 100), 
			100, 
			0, 
			nullptr, 
			asd::AlphaBlendMode::Blend,
			10000);
	}

	long CoreCircleCollider_Imp::GetCreationId() const {
		return creationId;
	}
}