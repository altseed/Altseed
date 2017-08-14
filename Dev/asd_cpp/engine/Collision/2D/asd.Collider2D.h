#pragma once

#include "../../asd.CoreToEngine.h"

namespace asd
{
	/**
	@brief	当たり判定や図形描画に用いられる図形を扱うクラス。
	*/
	class Collider2D
	{
		friend class Object2D;
		std::weak_ptr<Object2D> ownerObject2D;
	protected:
		virtual std::shared_ptr<CoreCollider2D> GetCoreCollider() const = 0;
	public:

		/**
		@brief	衝突判定図形のタイプを取得する。
		@return	衝突判定図形のタイプ
		*/
		virtual Collider2DShapeType GetType() const = 0;

		std::weak_ptr<Object2D> GetOwnerObject() const;

		/**
		@brief	このコライダのタグを取得する
		@return	コライダのタグ
		*/
		const achar* GetTag() const;

		/**
		@brief	このコライダのタグを設定する
		@param	tag	コライダのタグ
		*/
		void SetTag(const achar* tag);

		/**
		@brief	コライダを可視化する成否を設定する（デバッグ用）
		@param	isVisible コライダを可視化するか否か
		*/
		void SetVisible(bool isVisible);

		/**
		@brief	コライダを可視化する成否を取得する（デバッグ用）
		@param	isVisible コライダを可視化するか否か
		*/
		bool GetVisible();
	};
};