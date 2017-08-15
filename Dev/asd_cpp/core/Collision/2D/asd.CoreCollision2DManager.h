#pragma once

#include <vector>
#include <unordered_set>
#include <set>
#include <memory>
#include "asd.CoreCollision2D.h"
#include "Culling2D_AABB.h"
#include "asd.CoreCollision2D.h"

namespace asd {

	using namespace std;
	class CoreCollider2D;
	class CoreCollider2D_Imp;

	class CoreCollision2DManager {

		class ColliderPair {
			CoreCollider2D_Imp* colliderA_Imp;
			CoreCollider2D_Imp* colliderB_Imp;
		public:
			ColliderPair(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB) {
				this->colliderA_Imp = colliderA;
				this->colliderB_Imp = colliderB;
			}

			CoreCollider2D_Imp* GetColliderA_Imp() {
				return colliderA_Imp;
			}

			CoreCollider2D_Imp* GetColliderB_Imp() {
				return colliderB_Imp;
			}

			//ここより引用: https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
			size_t GetHashCode() const {
				size_t res = 17;
				res = res * 31 + std::hash<CoreCollider2D_Imp*>()(colliderA_Imp);
				res = res * 31 + std::hash<CoreCollider2D_Imp*>()(colliderB_Imp);
				return res;
			}

			bool operator==(const ColliderPair &contact) const{
				return colliderA_Imp == contact.colliderA_Imp && colliderB_Imp == contact.colliderB_Imp ||
					colliderA_Imp == contact.colliderB_Imp && colliderB_Imp == contact.colliderA_Imp;
			}


			bool operator!=(const ColliderPair &contact) const{
				return (colliderA_Imp != contact.colliderA_Imp || colliderB_Imp != contact.colliderB_Imp) &&
					(colliderA_Imp != contact.colliderB_Imp || colliderB_Imp != contact.colliderA_Imp);
			}
		};

		struct CoreCollision2DHashFunctor
		{
			size_t operator ()(ColliderPair v) const { return v.GetHashCode(); }
		};


		unordered_set<CoreCollider2D*> addedColliders;
		unordered_set<CoreCollider2D_Imp*> addedCollidersImp;
		unordered_map<ColliderPair,CoreCollision2D*, CoreCollision2DHashFunctor> addedCollisions;
		unordered_set<CoreCollider2D_Imp*> lastTransformedColliders;
		culling2d_aabb::World *cullingWorld;
	public:

		CoreCollision2DManager();
		virtual ~CoreCollision2DManager();

		void AddCollider(CoreCollider2D* collider);
		void RemoveCollider(CoreCollider2D* collider);

		void NotifyLastTransformed(CoreCollider2D_Imp* collider);

		void UpdateCollisionState();
	};
}