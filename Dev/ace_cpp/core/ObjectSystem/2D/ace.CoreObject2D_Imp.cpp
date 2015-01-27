
#include "ace.CoreObject2D_Imp.h"
#include "../../Graphics/ace.Graphics_Imp.h"
#include "ace.CoreTextureObject2D_Imp.h"
#include "ace.CoreCameraObject2D_Imp.h"
#include "ace.CoreTextObject2D_Imp.h"
#include "ace.CoreMapObject2D_Imp.h"
#include "ace.CoreEffectObject2D_Imp.h"

namespace ace
{

	CoreObject2D_Imp::CoreObject2D_Imp(Graphics_Imp* graphics)
		: m_graphics(graphics)
		, m_objectInfo(ObjectInfo2D())
		, m_transform(TransformInfo2D())
		, cullingObject(nullptr)
	{
		SafeAddRef(m_graphics);
		alreadyCullingUpdated = false;
	}

	CoreObject2D_Imp::~CoreObject2D_Imp()
	{
		SafeRelease(m_graphics);
	}

	culling2d::Circle& CoreObject2D_Imp::GetBoundingCircle()
	{
		return m_boundingCircle;
	}

	CoreObject2D_Imp* CoreObject2D_Imp::CoreObject2DToImp(CoreObject2D* obj)
	{
		if (obj == nullptr) return nullptr;

		switch (obj->GetObjectType())
		{
		case Object2DType::Texture:
			return (CoreTextureObject2D_Imp*)obj;
		case Object2DType::Text:
			return (CoreTextObject2D_Imp*)obj;
		case Object2DType::Camera:
			return (CoreCameraObject2D_Imp*)obj;
		case Object2DType::Map:
			return (CoreMapObject2D_Imp*)obj;
		case Object2DType::Effect:
			return (CoreEffectObject2D_Imp*)obj;
		default:
			break;
		}

		return nullptr;
	}
}