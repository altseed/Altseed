
#pragma once

#include "../ace.CoreLayer.h"

namespace ace
{
	class CoreLayer3D
		: public CoreLayer
	{
	protected:

		/**
		@brief	コンストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		CoreLayer3D(){}

		/**
		@brief	デストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		virtual ~CoreLayer3D(){}

	public:
		typedef CoreObject3D* ObjectPtr;

		virtual void AddObject(ObjectPtr object3D) = 0;
		virtual void RemoveObject(ObjectPtr object3D) = 0;
	};
}