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
		currentFrameContact = b2TestOverlap(collider1B2Shape, 0, collider2B2Shape, 0, b2Transform(), b2Transform());

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
#if !SWIG
	CoreCollision2D* CoreCollision2D::Create(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB) {
		auto contact = new CoreCollision2D();
		contact->colliderA = CoreCollider2D_Imp::CoreCollider2DImpToAbstract(colliderA);
		contact->colliderB = CoreCollider2D_Imp::CoreCollider2DImpToAbstract(colliderB);
		contact->colliderA_Imp = colliderA;
		contact->colliderB_Imp = colliderB;
		contact->currentFrameContact = false;
		contact->previousFrameContact = false;
		contact->isAlreadyExit = false;
		return contact;
	}

	void CoreCollision2D::Destroy(CoreCollision2D* contact) {
		delete contact;
	}

	bool CoreCollision2D::GetIsShouldDestroy() {
		return isShouldDestroy;
	}

	bool CoreCollision2D::operator==(CoreCollision2D *contact) {
		return colliderA_Imp == contact->colliderA_Imp && colliderB_Imp == contact->colliderB_Imp ||
			colliderA_Imp == contact->colliderB_Imp && colliderB_Imp == contact->colliderA_Imp;
	}


	bool CoreCollision2D::operator!=(CoreCollision2D *contact) {
		return (colliderA_Imp != contact->colliderA_Imp || colliderB_Imp != contact->colliderB_Imp) &&
			(colliderA_Imp != contact->colliderB_Imp || colliderB_Imp != contact->colliderA_Imp);
	}

#endif
}