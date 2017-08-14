
#include "asd.ObjectSystemFactory_Imp.h"

#include "2D/asd.CoreChip2D_Imp.h"
#include "2D/asd.CoreTextureObject2D_Imp.h"
#include "2D/asd.CoreCameraObject2D_Imp.h"
#include "2D/asd.CoreTextObject2D_Imp.h"
#include "2D/asd.CoreEffectObject2D_Imp.h"
#include "2D/asd.CoreMapObject2D_Imp.h"
#include "2D/asd.CoreGeometryObject2D_Imp.h"
#include "2D/asd.CoreLayer2D_Imp.h"

#include "3D/asd.CoreLayer3D_Imp.h"
#include "3D/asd.CoreCameraObject3D_Imp.h"
#include "3D/asd.CoreDirectionalLightObject3D_Imp.h"
#include "3D/asd.CoreEffectObject3D_Imp.h"
#include "3D/asd.CoreModelObject3D_Imp.h"
#include "3D/asd.CoreMassModelObject3D_Imp.h"
#include "3D/asd.CoreTerrainObject3D_Imp.h"

#include "../Core/asd.Core_Imp.h"

#include "asd.CoreScene_Imp.h"

#include "PostEffect/asd.CorePostEffect_Imp.h"

#include "Transition/asd.CoreTransition_Imp.h"

#include "../Log/asd.Log_Imp.h"

#include "../Graphics/asd.Graphics_Imp.h"


#include "../Shape/asd.CoreCircleShape_Imp.h"
#include "../Shape/asd.CoreArcShape_Imp.h"
#include "../Shape/asd.CoreLineShape_Imp.h"
#include "../Shape/asd.CoreRectangleShape_Imp.h"
#include "../Shape/asd.CoreTriangleShape_Imp.h"
#include "../Shape/asd.CorePolygonShape_Imp.h"

#include "../Collision/2D/asd.CoreCircleCollider_Imp.h"
#include "../Collision/2D/asd.CoreLineCollider_Imp.h"
#include "../Collision/2D/asd.CoreRectangleCollider_Imp.h"
#include "../Collision/2D/asd.CorePolygonCollider_Imp.h"

namespace asd
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory_Imp::ObjectSystemFactory_Imp(Core_Imp* core, Graphics_Imp* graphics, Log_Imp* logger, Vector2DI windowSize)
		: core(core)
		, m_graphics(graphics)
		, m_logger(logger)
		, m_windowSize(windowSize)
	{
		SafeAddRef(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory_Imp::~ObjectSystemFactory_Imp()
	{
		SafeRelease(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreTextureObject2D* ObjectSystemFactory_Imp::CreateTextureObject2D()
	{
		return new CoreTextureObject2D_Imp(m_graphics);
	}

	CoreCameraObject2D* ObjectSystemFactory_Imp::CreateCameraObject2D()
	{
		return new CoreCameraObject2D_Imp(m_graphics);
	}

	CoreTextObject2D* ObjectSystemFactory_Imp::CreateTextObject2D()
	{
		return new CoreTextObject2D_Imp(m_graphics);
	}

	CoreEffectObject2D* ObjectSystemFactory_Imp::CreateEffectObject2D()
	{
		return new CoreEffectObject2D_Imp(m_graphics);
	}

	CoreMapObject2D* ObjectSystemFactory_Imp::CreateMapObject2D()
	{
		return new CoreMapObject2D_Imp(m_graphics);
	}

	CoreGeometryObject2D* ObjectSystemFactory_Imp::CreateGeometryObject2D()
	{
		return new CoreGeometryObject2D_Imp(m_graphics);
	}

	CoreChip2D* ObjectSystemFactory_Imp::CreateChip2D()
	{
		return new CoreChip2D_Imp(m_graphics);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D* ObjectSystemFactory_Imp::CreateLayer2D()
	{
		return new CoreLayer2D_Imp(core, m_graphics, m_logger, m_windowSize);
	}

	CoreModelObject3D* ObjectSystemFactory_Imp::CreateModelObject3D()
	{
		return new CoreModelObject3D_Imp(m_graphics);
	}

	CoreCameraObject3D* ObjectSystemFactory_Imp::CreateCameraObject3D()
	{
		return new CoreCameraObject3D_Imp(m_graphics);
	}

	CoreEffectObject3D* ObjectSystemFactory_Imp::CreateEffectObject3D()
	{
		return new CoreEffectObject3D_Imp(m_graphics);
	}

	CoreDirectionalLightObject3D* ObjectSystemFactory_Imp::CreateDirectionalLightObject3D()
	{
		return new CoreDirectionalLightObject3D_Imp(m_graphics);
	}

	CoreMassModelObject3D* ObjectSystemFactory_Imp::CreateCoreMassModelObject3D()
	{
		return new CoreMassModelObject3D_Imp(m_graphics);
	}

	CoreTerrainObject3D* ObjectSystemFactory_Imp::CreateCoreTerrainObject3D()
	{
		return new CoreTerrainObject3D_Imp(m_graphics);
	}

	CoreLayer3D* ObjectSystemFactory_Imp::CreateLayer3D(RenderSettings settings)
	{
		return new CoreLayer3D_Imp(core, m_graphics, m_logger, m_windowSize, settings);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene* ObjectSystemFactory_Imp::CreateScene()
	{
		return new CoreScene_Imp(m_graphics, m_windowSize);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CorePostEffect* ObjectSystemFactory_Imp::CreatePostEffect()
	{
		return new CorePostEffect_Imp(m_graphics);
	}


	CoreTransition* ObjectSystemFactory_Imp::CreateTransition()
	{
		return new CoreTransition_Imp();
	}

	CoreArcShape* ObjectSystemFactory_Imp::CreateArcShape()
	{
		return new CoreArcShape_Imp();
	}

	CoreCircleShape* ObjectSystemFactory_Imp::CreateCircleShape()
	{
		return new CoreCircleShape_Imp();
	}

	CoreLineShape* ObjectSystemFactory_Imp::CreateLineShape()
	{
		return new CoreLineShape_Imp();
	}

	CoreRectangleShape* ObjectSystemFactory_Imp::CreateRectangleShape()
	{
		return new CoreRectangleShape_Imp();
	}

	CoreTriangleShape* ObjectSystemFactory_Imp::CreateTriangleShape()
	{
		return new CoreTriangleShape_Imp();
	}

	CorePolygonShape* ObjectSystemFactory_Imp::CreatePolygonShape()
	{
		return new CorePolygonShape_Imp();
	}

	CoreCircleCollider* ObjectSystemFactory_Imp::CreateCircleCollider()
	{
		return new CoreCircleCollider_Imp();
	}

	CoreLineCollider* ObjectSystemFactory_Imp::CreateLineCollider()
	{
		return new CoreLineCollider_Imp();
	}

	CoreRectangleCollider* ObjectSystemFactory_Imp::CreateRectangleCollider()
	{
		return new CoreRectangleCollider_Imp();
	}

	CorePolygonCollider* ObjectSystemFactory_Imp::CreatePolygonCollider()
	{
		return new CorePolygonCollider_Imp();
	}
}