#pragma once

#include "../../asd.CoreToEngine.h"

namespace asd
{

	class Scene;
	class Object2D;
	class Object3D;

	/**
	@brief	2Dのコライダクラス
	*/
	class Collider2D
	{
		friend class Object2D;
		std::weak_ptr<Object2D> ownerObject2D;
	protected:
		virtual std::shared_ptr<CoreCollider2D> GetCoreCollider() const = 0;
	public:

		/**
		@brief	コライダの形状を取得する。
		@return	コライダの形状
		*/
		virtual Collider2DShapeType GetShapeType() const = 0;

		/**
		@brief	このコライダを保持しているObject2Dを取得する
		*/
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
		@brief	このコライダを可視化する成否を設定する（デバッグ用）
		@param	isVisible コライダの可視化の成否
		*/
		void SetVisible(bool isVisible);

		/**
		@brief	このコライダを可視化する成否を取得する（デバッグ用）
		@return コライダの可視化の成否
		*/
		bool GetVisible();
	};
};