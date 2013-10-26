#pragma once
#include <memory>
#include "ace.CoreLayer.h"
#include "../ace.ReferenceObject.h"

namespace ace
{
	class CoreScene : public IReference
	{
	protected:
		CoreScene() {}
		virtual ~CoreScene() {}

	public:
		typedef CoreLayer* LayerPtr;
		
		virtual void AddLayer(LayerPtr layer) = 0;
		virtual void RemoveLayer(LayerPtr layer) = 0;

		virtual void Draw() = 0;
	};
}