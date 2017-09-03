#include "asd.CoreCollider2D_Imp.h"
#include "../Box2D/Box2D.h"
#include "asd.CoreCircleCollider_Imp.h"
#include "asd.CoreLineCollider_Imp.h"
#include "asd.CorePolygonCollider_Imp.h"
#include "asd.CoreRectangleCollider_Imp.h"
#include "../../Collision/2D/asd.CoreCollider2D_Imp.h"

namespace asd {

	int64_t CoreCollider2D_Imp::currentCreationId = 0;

	int64_t CoreCollider2D_Imp::GetCurrentCreationId() {
		return currentCreationId++;
	}

	CoreCollider2D_Imp::CoreCollider2D_Imp()
		:isNeededUpdateShapes(false)
		, ownerObject(nullptr)
		, coreCollision2DManager(nullptr)
	{
		creationId = GetCurrentCreationId();
	}

	CoreCollider2D_Imp::~CoreCollider2D_Imp() {

	}

	void CoreCollider2D_Imp::SetOwnerObject2D(CoreObject2D_Imp* ownerObject) {
		this->ownerObject = ownerObject;
	}

	CoreObject2D_Imp* CoreCollider2D_Imp::GetOwnerObject2D() {
		return this->ownerObject;
	}

	CoreCollider2D_Imp* CoreCollider2D_Imp::CoreCollider2DToImp(CoreCollider2D* obj)
	{
		if (obj == nullptr) return nullptr;

		switch (obj->GetShapeType())
		{
		case Collider2DShapeType::Circle:
			return (CoreCircleCollider_Imp*)obj;
		case Collider2DShapeType::Line:
			return (CoreLineCollider_Imp*)obj;
		case Collider2DShapeType::Polygon:
			return (CorePolygonCollider_Imp*)obj;
		case Collider2DShapeType::Rectangle:
			return (CoreRectangleCollider_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}

	CoreCollider2D* CoreCollider2D_Imp::CoreCollider2DImpToAbstract(CoreCollider2D_Imp* obj) {
		if (obj == nullptr) return nullptr;

		switch (obj->GetShapeType())
		{
		case Collider2DShapeType::Circle:
			return (CoreCircleCollider_Imp*)obj;
		case Collider2DShapeType::Line:
			return (CoreLineCollider_Imp*)obj;
		case Collider2DShapeType::Polygon:
			return (CorePolygonCollider_Imp*)obj;
		case Collider2DShapeType::Rectangle:
			return (CoreRectangleCollider_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}

	void CoreCollider2D_Imp::OnAddedToObject(CoreObject2D_Imp* ownerObject) {
		SetOwnerObject2D(ownerObject);

		if (coreCollision2DManager != nullptr) {
			coreCollision2DManager->NotifyLastTransformed(this);
		}
	}

	void CoreCollider2D_Imp::OnRemovedFromObject(CoreObject2D_Imp* ownerObject) {
		SetOwnerObject2D(nullptr);
	}

	void CoreCollider2D_Imp::OnOwnerObjectTransformChanged(CoreObject2D_Imp*) {
		if (coreCollision2DManager != nullptr) {
			coreCollision2DManager->NotifyLastTransformed(this);
		}
	}

	void CoreCollider2D_Imp::SetCullingObject(culling2d_aabb::Object* object) {
		cullingObject = object;
	}

	culling2d_aabb::Object* CoreCollider2D_Imp::GetCullingObject() {
		return cullingObject;
	}

	culling2d_aabb::RectF CoreCollider2D_Imp::GetAABB() {
		return aabb;
	}

	void CoreCollider2D_Imp::SetAABB(culling2d_aabb::RectF aabb) {
		this->aabb = aabb;
	}
	void CoreCollider2D_Imp::SetCollisionManager(CoreCollision2DManager *collision2DManager) {
		this->coreCollision2DManager = collision2DManager;
	}

	CoreCollision2DManager* CoreCollider2D_Imp::GetCollisionManager() {
		return coreCollision2DManager;
	}
}