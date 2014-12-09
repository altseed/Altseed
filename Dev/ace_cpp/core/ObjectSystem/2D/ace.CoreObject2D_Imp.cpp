
#include "ace.CoreObject2D_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"

namespace ace
{
	CoreObject2D_Imp::CoreObject2D_Imp(Graphics_Imp* graphics)
		: m_graphics(graphics)
		, m_objectInfo(ObjectInfo2D())
		, m_transform(TransformInfo2D())
	{
		SafeAddRef(m_graphics);
	}

	CoreObject2D_Imp::~CoreObject2D_Imp()
	{
		SafeRelease(m_graphics);
	}

	culling2d::Circle& CoreObject2D_Imp::GetBoundingCircle()
	{
		return m_boundingCircle;
	}
}