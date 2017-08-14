#pragma once

#include <vector>
#include <memory>
#include "asd.Collider2D.h"

namespace asd {
	class Collision2DInfo {
		std::shared_ptr<Collider2D> selfCollider;
		std::shared_ptr<Collider2D> theirsCollider;


	public:
		Collision2DInfo(std::shared_ptr<Collider2D> selfCollider, std::shared_ptr<Collider2D> theirsCollider) {
			this->selfCollider = selfCollider;
			this->theirsCollider = theirsCollider;
		}

		virtual ~Collision2DInfo() {}

		std::shared_ptr<Collider2D> GetSelfCollider() {
			return selfCollider;
		}

		std::shared_ptr<Collider2D> GetTheirsCollider() {
			return theirsCollider;
		}
	};
}
