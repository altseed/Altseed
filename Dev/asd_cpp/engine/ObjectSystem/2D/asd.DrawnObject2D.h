#pragma once
#include <memory>
#include "asd.Object2D.h"

namespace asd
{
	class DrawnObject2D : public Object2D
	{
	private:
		virtual CoreDrawnObject2D* GetCoreDrawnObject() const = 0;

	public:
		typedef std::shared_ptr<DrawnObject2D> Ptr;

		/**
			@brief	描画する際に合成する色を取得する。
		*/
		Color GetColor() const;
		/**
			@brief	描画する際に合成する色を設定する。
		*/
		void SetColor(Color value);

		/**
			@brief	このオブジェクトの描画優先度を取得する。
		*/
		int GetDrawingPriority() const;
		/**
			@brief	このオブジェクトの描画優先度を設定する。
		*/
		void SetDrawingPriority(int value);

		/**
			@brief	親子関係を考慮して最終的に描画時にこのオブジェクトに合成する色を取得する。
		*/
		Color GetAbsoluteColor() const;
		/**
			@brief	親子関係を考慮したこのオブジェクトの最終的な描画優先度を取得する。
		*/
		int GetAbsoluteDrawingPriority() const;

		/**
			@brief	描画に関する同期設定を指定して、指定した2Dオブジェクトを子オブジェクトとしてこのインスタンスに追加する。
			@param	child	追加する子オブジェクト
			@param	managementMode	子オブジェクトの管理に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
			@param	transformingmode	子オブジェクトの変形に関する同期設定。
			@param	drawingMode	子オブジェクトの描画に関する同期設定。フラグをOR演算でつなげて複数指定することができる。
		*/
		void AddDrawnChild(const DrawnObject2D::Ptr& child,
			ChildManagementMode::Flags managementMode,
			ChildTransformingMode transformingMode,
			ChildDrawingMode::Flags drwingMode);
	};
}