#pragma once
#include <list>
#include "ace.CoreObject2D.h"
#include "../../Graphics/2D/ace.Renderer2D_Imp.h"
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

		virtual void DrawSpriteAdditionally(Vector2DF pos1, Vector2DF pos2, Vector2DF pos3, Vector2DF pos4,
			Color col1, Color col2, Color col3, Color col4,
			Vector2DF uv1, Vector2DF uv2, Vector2DF uv3, Vector2DF uv4,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) = 0;

#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
		virtual Graphics* GetGraphicsImp() = 0;
#endif
	};
}