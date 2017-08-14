#pragma once

#include "asd.Collider2D.h"

namespace asd {

	class CircleCollider : public Collider2D {
	private:
		std::shared_ptr<CoreCircleCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;

	public:

		CircleCollider();
		virtual ~CircleCollider();

		/**
		@brief	この円の中心座標を取得する。
		@return	この円の中心座標
		*/
		Vector2DF GetCenter() const;

		/**
		@brief	この円の中心座標を設定する。
		@param	position	この円の中心座標
		*/
		void SetCenter(Vector2DF position);

		/**
		@brief	この円の半径を取得する。
		@return	この円の半径
		*/
		float GetRadius() const;

		/**
		@brief	この円の半径を設定する。
		@param	radius	この円の半径
		*/
		void SetRadius(float radius);

		Collider2DShapeType GetType() const;
	};
}
