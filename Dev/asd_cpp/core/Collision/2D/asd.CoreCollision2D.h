#pragma once

#include <unordered_map>

namespace asd {
	class CoreCollider2D;
	class CoreCollider2D_Imp;

	class CoreCollision2D {
	private:
		bool currentFrameContact;
		bool previousFrameContact;
		bool isAlreadyExit;

		CoreCollider2D* colliderA;
		CoreCollider2D* colliderB;

		bool isShouldDestroy;

		CoreCollision2D():
			currentFrameContact(false),
			previousFrameContact(false),
			isShouldDestroy(false)
		{}
		virtual ~CoreCollision2D() {}


#if !SWIG
	private:
		CoreCollider2D_Imp* colliderA_Imp;
		CoreCollider2D_Imp* colliderB_Imp;
	public:
		bool GetIsShouldDestroy();

		static CoreCollision2D* Create(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB);
		static void Destroy(CoreCollision2D* contact);
		bool operator==(CoreCollision2D *contact);
		bool operator!=(CoreCollision2D *contact);
		

		size_t GetHashCode() const {
			return std::hash<CoreCollider2D_Imp*>()(colliderA_Imp) ^ std::hash<CoreCollider2D_Imp*>()(colliderB_Imp);
		}
		
		struct HashFunctor
		{
			size_t operator ()(const CoreCollision2D* v) const { return v->GetHashCode(); }
		};

		CoreCollider2D_Imp* GetColliderA_Imp() {
			return colliderA_Imp;
		}

		CoreCollider2D_Imp* GetColliderB_Imp() {
			return colliderB_Imp;
		}
#endif
	public:
		void Update();

		CoreCollider2D* GetColliderA() {
			return colliderA;
		}

		CoreCollider2D* GetColliderB() {
			return colliderB;
		}
	};
}