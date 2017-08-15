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
		private:
			//boost::hash_combineより引用
			inline size_t CombineHash(size_t seed, size_t value)
			{
				return seed ^(value + 0x9e3779b9 + (seed << 6) + (seed >> 2));
			}
			CoreCollider2D_Imp* colliderA_Imp;
			CoreCollider2D_Imp* colliderB_Imp;
			size_t hashValue;
		public:
			ColliderPair(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB) {
				this->colliderA_Imp = colliderA;
				this->colliderB_Imp = colliderB;
				auto hashA = std::hash<CoreCollider2D_Imp*>()(colliderA_Imp);
				auto hashB = std::hash<CoreCollider2D_Imp*>()(colliderB_Imp);

				if (hashA > hashB) {
					size_t seed = 0;
					seed = CombineHash(seed, hashA);
					hashValue = CombineHash(seed, hashB);
				}
				else {
					size_t seed = 0;
					seed = CombineHash(seed, hashB);
					hashValue = CombineHash(seed, hashA);
				}
			}

			CoreCollider2D_Imp* GetColliderA_Imp() {
				return colliderA_Imp;
			}

			CoreCollider2D_Imp* GetColliderB_Imp() {
				return colliderB_Imp;
			}

			size_t GetHashCode() const {
				return hashValue;
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