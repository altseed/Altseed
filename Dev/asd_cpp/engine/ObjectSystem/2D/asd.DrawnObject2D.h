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

		virtual Color GetColor() const = 0;
		virtual void SetColor(Color value) = 0;

		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual Color GetAbsoluteColor() const = 0;
		virtual int GetAbsoluteDrawingPriority() const = 0;

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