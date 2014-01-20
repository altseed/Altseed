#pragma once
#include <list>
#include "ace.CoreObject2D.h"
#include "../../Graphics/Common/2D/ace.Renderer2D_Imp.h"
#include "../ace.CoreLayer.h"

namespace ace
{
	class CoreLayer2D
		: public CoreLayer
	{
	public:
		typedef CoreObject2D* ObjectPtr;

		/**
			@brief	コンストラクタ
			@note
			中身がなくともないと多重継承に失敗する。
		*/
		CoreLayer2D(){}

		/**
		@brief	デストラクタ
		@note
		中身がなくともないと多重継承に失敗する。
		*/
		virtual ~CoreLayer2D(){}

		virtual void AddObject(ObjectPtr object2D) = 0;
		virtual void RemoveObject(ObjectPtr object2D) = 0;

#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
		virtual Graphics* GetGraphicsImp() = 0;
#endif
	};
}