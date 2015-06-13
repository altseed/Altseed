#pragma once

#include "asd.Shape.h"

namespace asd
{
	/**
	@brief	図形としての円を扱うクラス。
	*/
	class CircleShape :public Shape
	{
	private:
		std::shared_ptr<CoreCircleShape> m_coreObject;

	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		CircleShape();
		virtual ~CircleShape();

		/**
		@brief	この円の中心座標を取得する。
		@return	この円の中心座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	この円の中心座標を設定する。
		@param	position	この円の中心座標
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief	この円の外径を取得する。
		@return	この円の外径
		*/
		float GetOuterDiameter() const;

		/**
		@brief	この円の外径を設定する。
		@param	outerDiameter	この円の外径
		*/
		void SetOuterDiameter(float outerDiameter);

		/**
		@brief	この円の内径を取得する。
		@return	この円の内径
		*/
		float GetInnerDiameter() const;

		/**
		@brief	この円の内径を設定する。
		@param	innerDiameter	この円の内径
		*/
		void SetInnerDiameter(float innerDiameter);

		/**
		@brief	この円の中心を軸とした回転角度を取得する。
		@return	この円の中心を軸とした回転角度
		*/
		float GetAngle() const;

		/**
		@brief	この円の中心を軸とした回転角度を設定する。
		@param	angle	この円の中心を軸とした回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	この円の外周、内周上の曲がり角（滑らかさ）の数を取得する。
		@return	外周、内周状の曲がり角の数
		*/
		int GetNumberOfCorners() const;

		/**
		@brief	この円の外周、内周上の曲がり角（滑らかさ）の数を設定する。
		@param	numberOfCorners	外周、内周状の曲がり角の数
		*/
		void SetNumberOfCorners(int numberOfCorners);

		ShapeType GetShapeType() const;
	};
}