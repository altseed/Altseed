#pragma once

#include "ace.ShapeType.h"
#include "../ace.ReferenceObject.h"

namespace ace{


	class CoreShape 
		: public IReference
	{
	public:
		CoreShape(){}
		virtual ~CoreShape(){}

		virtual ShapeType GetShapeType() const = 0;

	};
};
