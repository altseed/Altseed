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

		bool isShouldDestroy;
		CoreCollision2D() {}

	public:

#if !SWIG
		CoreCollision2D(CoreCollider2D* colliderA, CoreCollider2D* colliderB);
		virtual ~CoreCollision2D();
	private:
		CoreCollider2D* colliderA;
		CoreCollider2D* colliderB;
	public:
		bool GetIsShouldDestroy();
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