
#pragma once

#include "../ace.ReferenceObject.h"
#include "ace.CoreScene.h"

namespace ace
{
	class CoreLayer
		: public IReference
	{
	protected:
		CoreLayer() {}
		virtual ~CoreLayer() {}
	public:
		
		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual bool GetIsDrawn() const = 0;
		virtual void SetIsDrawn(bool value) = 0;

		virtual void AddPostEffect(CorePostEffect* postEffect) = 0;
		virtual void ClearPostEffects() = 0;

		virtual void BeginUpdating(bool isUpdated) = 0;
		virtual void EndUpdating(bool isUpdated) = 0;

		virtual void BeginDrawing() = 0;
		virtual void Draw() = 0;
		virtual void EndDrawing() = 0;

		virtual void BeginMeasureUpdateTime() = 0;
		virtual void EndMeasureUpdateTime() = 0;
		virtual int GetTimeForUpdate() const = 0;

#if !SWIG
		virtual void SetScene(CoreScene* scene) = 0;
#endif
	};
}