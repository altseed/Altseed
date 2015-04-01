#include "ace.CoreShape.h"
#include "ace.CoreTriangle.h"

namespace ace
{
	CoreShape::~CoreShape()
	{
		for (auto triangle : triangles)
		{
			SafeRelease(triangle);
		}
	}

#if !SWIG
	std::vector<CoreTriangle*>& CoreShape::GetDividedTriangles()
	{
		if (isNeededUpdating)
		{
			for (auto triangle : triangles)
			{
				SafeRelease(triangle);
			}
			triangles.clear();
			DivideToTriangles();
			isNeededUpdating = false;
		}
		return triangles;
	}

	culling2d::Circle& CoreShape::GetBoundingCircle()
	{
		if (isNeededCalcBoundingCircle)
		{
			CalculateBoundingCircle();
			isNeededCalcBoundingCircle = false;
		}
		return boundingCircle;
	}
#endif
};