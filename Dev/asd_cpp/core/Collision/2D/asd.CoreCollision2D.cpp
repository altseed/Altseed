#include "asd.CoreCollision2D.h"
#include "asd.CoreCollider2D_Imp.h"

namespace asd {


	void CoreCollision2D::Update() {

		auto aabb1 = colliderA_Imp->GetAABB();
		auto aabb2 = colliderB_Imp->GetAABB();

		if (isShouldDestroy) {
			return;
		}

		if (!aabb1.GetCollision(aabb2) && isAlreadyExit) {
			isShouldDestroy = true;
		}

		auto collider1B2Shape = colliderA_Imp->GetB2Shape();
		auto collider2B2Shape = colliderB_Imp->GetB2Shape();

		previousFrameContact = currentFrameContact;

		b2Transform identity = b2Transform();
		identity.SetIdentity();

		currentFrameContact = b2TestOverlap(collider1B2Shape, 0, collider2B2Shape, 0, identity, identity);

		auto ownerA = colliderA_Imp->GetOwnerObject2D();
		auto ownerB = colliderB_Imp->GetOwnerObject2D();

		if (!previousFrameContact && currentFrameContact) { // Enter
			auto event = CoreCollision2DEvent(CollisionType::Enter, this);
			ownerA->currentFrameCollisionEvents.push_back(event);
			ownerB->currentFrameCollisionEvents.push_back(event);
		}
		else if (previousFrameContact && currentFrameContact) { //Stay
			auto event = CoreCollision2DEvent(CollisionType::Stay, this);
			ownerA->currentFrameCollisionEvents.push_back(event);
			ownerB->currentFrameCollisionEvents.push_back(event);
		}
		else if (previousFrameContact && !currentFrameContact) { //Exit
			auto event = CoreCollision2DEvent(CollisionType::Exit, this);
			ownerA->currentFrameCollisionEvents.push_back(event);
			ownerB->currentFrameCollisionEvents.push_back(event);
			isAlreadyExit = true;
		}

	}
	CoreCollision2D::CoreCollision2D(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB) :
		currentFrameContact(false),
		previousFrameContact(false),
		isAlreadyExit(false),
		isShouldDestroy(false)
	{
		this->colliderA = CoreCollider2D_Imp::CoreCollider2DImpToAbstract(colliderA);
		this->colliderB = CoreCollider2D_Imp::CoreCollider2DImpToAbstract(colliderB);
		this->colliderA_Imp = colliderA;
		this->colliderB_Imp = colliderB;
	}

#if !SWIG

	bool CoreCollision2D::GetIsShouldDestroy() {
		return isShouldDestroy;
	}

#endif
}