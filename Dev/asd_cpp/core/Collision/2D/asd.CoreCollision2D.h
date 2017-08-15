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
		CoreCollision2D() {}

	public:

#if !SWIG
		CoreCollision2D(CoreCollider2D_Imp* colliderA, CoreCollider2D_Imp* colliderB);
		virtual ~CoreCollision2D() {}
	private:
		CoreCollider2D_Imp* colliderA_Imp;
		CoreCollider2D_Imp* colliderB_Imp;
	public:
		bool GetIsShouldDestroy();
		

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