#include "ace.CoreShape.h"
#include "ace.CoreTriangle.h"

namespace ace
{
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
#endif
};