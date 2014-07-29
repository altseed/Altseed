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
		virtual void Clear() = 0;

		virtual void DrawSpriteAdditionally(Vector2DF upperLeftPos, Vector2DF upperRightPos, Vector2DF lowerRightPos, Vector2DF lowerLeftPos,
			Color upperLeftCol, Color upperRightCol, Color lowerRightCol, Color lowerLeftCol,
			Vector2DF upperLeftUV, Vector2DF upperRightUV, Vector2DF lowerRightUV, Vector2DF lowerLeftUV,
			Texture2D* texture, AlphaBlend alphaBlend, int32_t priority) = 0;

#if !SWIG
		virtual Renderer2D* GetRenderer() const = 0;
		virtual Graphics* GetGraphicsImp() = 0;
#endif
	};
}