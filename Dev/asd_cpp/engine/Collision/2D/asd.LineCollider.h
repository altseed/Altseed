#pragma once

#include "asd.Collider2D.h"

namespace asd {

	/**
	@brief	線形のコライダクラス
	*/
	class LineCollider : public Collider2D{
	private:
		std::shared_ptr<CoreLineCollider> coreObject;
		std::shared_ptr<CoreCollider2D> GetCoreCollider() const override;

	public:

		LineCollider();
		virtual ~LineCollider();

		/**
		@brief	この線分の始点を取得する
		@return	この線分の始点
		*/
		Vector2DF GetStartingPosition() const;

		/**
		@brief	この線分の始点を設定する
		@param	startingPosition	この線分の始点
		*/
		void SetStartingPosition(Vector2DF startingPosition);

		/**
		@brief	この線分の終点を取得する
		@return	この線分の終点
		*/
		Vector2DF GetEndingPosition() const;

		/**
		@brief	この線分の終点を設定する
		@param	endingPosition	この線分の終点
		*/
		void SetEndingPosition(Vector2DF endingPosition);

		/**
		@brief	この線分の太さを設定する
		@return	この線分の太さ
		*/
		float GetThickness() const;

		/**
		@brief	この線分の太さを取得する
		@param	thickness	この線分の太さ
		*/
		void SetThickness(float thickness);

		Collider2DShapeType GetShapeType() const override;
	};
}
