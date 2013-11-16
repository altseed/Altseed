
#pragma once

#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreLayer : public IReference
	{
	protected:
		CoreLayer() {}
		virtual ~CoreLayer() {}
	public:
		
		virtual int GetDrawingPriority() const = 0;
		virtual void SetDrawingPriority(int value) = 0;

		virtual void Draw() = 0;

		virtual bool GetIsDrawn() const = 0;
		virtual void SetIsDrawn(bool value) = 0;
	};
}