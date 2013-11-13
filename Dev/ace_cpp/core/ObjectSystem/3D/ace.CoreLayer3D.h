
#pragma once

#include "../ace.CoreLayer.h"

namespace ace
{
	class CoreLayer3D
		: public CoreLayer
	{
	public:
		typedef CoreObject3D* ObjectPtr;

		virtual void AddObject(ObjectPtr object3D) = 0;
		virtual void RemoveObject(ObjectPtr object3D) = 0;
	};
}