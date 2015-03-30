#pragma once

#include "ace.ShapeType.h"
#include "../ace.ReferenceObject.h"
namespace ace{

	class CoreTriangle;

	class CoreShape 
		: public IReference
	{
	public:
		CoreShape(){}
		virtual ~CoreShape(){}

		virtual ShapeType GetShapeType() const = 0;

#if !SWIG
		virtual std::vector<CoreTriangle*> GetDividedTriangles() = 0;
#endif
	};
};
