#pragma once

#include "asd.CoreCollision2D.h"

namespace asd {

	enum class CollisionType {
		Enter,
		Stay,
		Exit
	};

	class CoreCollision2DEvent {
		CollisionType collisionType;
		CoreCollision2D* collision;
	public:

		CoreCollision2DEvent(CollisionType collisionType, CoreCollision2D* collision) {
			this->collisionType = collisionType;
			this->collision = collision;
		}

		CollisionType GetCollisionType() {
			return collisionType;
		}

		CoreCollision2D* GetCollision(){
			return collision;
		}
	};
}
