#pragma once

#include <vector>
#include <memory>
#include "asd.Collider2D.h"

namespace asd {

	/**
	2Dコライダの衝突情報を扱うクラス
	*/
	class Collision2DInfo {
		std::shared_ptr<Collider2D> selfCollider;
		std::shared_ptr<Collider2D> theirsCollider;
		CollisionType collisionType;

	public:
		Collision2DInfo(std::shared_ptr<Collider2D> selfCollider, std::shared_ptr<Collider2D> theirsCollider, CollisionType collisionType) {
			this->selfCollider = selfCollider;
			this->theirsCollider = theirsCollider;
			this->collisionType = collisionType;
		}

		virtual ~Collision2DInfo() {}

		/**
		brief	衝突した自分自身のコライダを取得する
		*/
		std::shared_ptr<Collider2D> GetSelfCollider() {
			return selfCollider;
		}

		/**
		@brief	衝突した相手のコライダを取得する
		*/
		std::shared_ptr<Collider2D> GetTheirsCollider() {
			return theirsCollider;
		}

		/**
		@brief	衝突状態の種類を取得する
		*/
		CollisionType GetCollisionType() {
			return collisionType;
		}
	};
}
