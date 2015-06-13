#pragma once
#include <memory>
#include "asd.Chip2D.h"
#include "asd.Object2D.h"

namespace asd
{
	/**
	@brief	テクスチャをチップとして高速に描画するクラス。
	*/
	class MapObject2D : public Object2D
	{
		friend class Chip2D;
	private:
		std::shared_ptr<CoreMapObject2D> m_coreObject;
		std::set<std::shared_ptr<Chip2D>> m_chips;

		CoreObject2D* GetCoreObject() const;

	public:
		typedef std::shared_ptr<MapObject2D> Ptr;

		/**
		@brief	コンストラクタ
		*/
		MapObject2D();
		/**
		@brief	デストラクタ
		*/
		virtual ~MapObject2D();

		/**
		@brief	このオブジェクトの原点位置を取得する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		Vector2DF GetCenterPosition() const;

		/**
		@brief	このオブジェクトの描画優先度を取得します。
		*/
		int GetDrawingPriority() const;

		/**
		@brief	このオブジェクトの原点位置を設定する。この位置が、描画する際の描画・拡縮・回転の中心となる。
		*/
		void SetCenterPosition(Vector2DF position);

		/**
		@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int priority);

		/**
		@brief	このオブジェクトに描画チップを追加する。
		*/
		bool AddChip(std::shared_ptr<Chip2D> chip);

		/**
		@brief	このオブジェクトから描画チップを削除する。
		*/
		bool RemoveChip(std::shared_ptr<Chip2D> chip);

		/**
		@brief	このオブジェクトに追加されている描画チップを全て削除する。
		*/
		void Clear();
	};
}