#pragma once

#include "ace.Shape.h"

namespace ace
{
	/**
	@brief	図形としての弧を扱うクラス。
	*/
	class ArcShape :public Shape
	{
	private:
		std::shared_ptr<CoreArcShape> m_coreObject;
	protected:
		std::shared_ptr<CoreShape> GetCoreShape() const override;

	public:
		ArcShape();
		virtual ~ArcShape();

		/**
		@brief	この弧の中心座標を取得する。
		@return	この弧の中心座標
		*/
		Vector2DF GetPosition() const;

		/**
		@brief	この弧の中心座標を設定する。
		@param	position	この弧の中心座標
		*/
		void SetPosition(Vector2DF position);

		/**
		@brief	この弧の外径を取得する。
		@return	この弧の外径
		*/
		float GetOuterDiameter() const;

		/**
		@brief	この弧の外径を設定する。
		@param	outerDiameter	この弧の外径
		*/
		void SetOuterDiameter(float outerDiameter);

		/**
		@brief	この弧の内径を取得する。
		@return	この弧の内径
		*/
		float GetInnerDiameter() const;

		/**
		@brief	この弧の内径を設定する。
		@param	innerDiameter	この弧の内径
		*/
		void SetInnerDiameter(float innerDiameter);

		/**
		@brief	この弧の中心を軸とした回転角度を取得する。
		@return	この弧の中心を軸とした回転角度
		*/
		float GetAngle() const;

		/**
		@brief	この弧の中心を軸とした回転角度を設定する。
		@param	angle	この弧の中心を軸とした回転角度
		*/
		void SetAngle(float angle);

		/**
		@brief	この弧の外周、内周上の曲がり角（滑らかさ）の数を取得する。
		@return	外周、内周状の曲がり角の数
		*/
		int GetNumberOfCorners() const;

		/**
		@brief	この弧の外周、内周上の曲がり角（滑らかさ）の数を設定する。
		@param	numberOfCorners	外周、内周状の曲がり角の数
		*/
		void SetNumberOfCorners(int numberOfCorners);

		/**
		@brief	この弧の始点となる外周、内周の曲がり角の番号を取得する。(0 <= StartingCorner <= NumberOfCorners)
		@return	始点となる外周、内周の曲がり角の番号
		*/
		int GetStartingCorner() const;

		/**
		@brief	この弧の始点となる外周、内周の曲がり角の番号を設定する。(0 <= StartingCorner <= NumberOfCorners)
		@param	startingCorner	始点となる外周、内周の曲がり角の番号
		*/
		void SetStartingCorner(int startingCorner);

		/**
		@brief	この弧の終点となる外周、内周の曲がり角の番号を取得する。(0 <= EndingCorner <= NumberOfCorners)
		@return	終点となる外周、内周の曲がり角の番号
		*/
		int GetEndingCorner() const;

		/**
		@brief	この弧の終点となる外周、内周の曲がり角の番号を設定する。(0 <= EndingCorner <= NumberOfCorners)
		@param	endingCorner	終点となる外周、内周の曲がり角の番号
		*/
		void SetEndingCorner(int endingCorner);

		ShapeType GetShapeType() const override;
	};
}