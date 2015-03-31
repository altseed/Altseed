#pragma once

#include "ace.ShapeType.h"
#include "../ace.Core.Base.h"

namespace ace{

	class CoreTriangle;

	class CoreShape 
		: public IReference
	{
#if !SWIG
	protected:
		std::vector<CoreTriangle*> triangles;
		bool isNeededUpdating;

		virtual void DivideToTriangles() = 0;
#endif
	public:
		CoreShape():isNeededUpdating(false){}
		virtual ~CoreShape(){}

		virtual ShapeType GetShapeType() const = 0;

#if !SWIG
		std::vector<CoreTriangle*>& GetDividedTriangles();
#endif
	};
};
