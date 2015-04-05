
#include "ace.ObjectSystemFactory_Imp.h"

#include "2D/ace.CoreTextureObject2D_Imp.h"
#include "2D/ace.CoreCameraObject2D_Imp.h"
#include "2D/ace.CoreTextObject2D_Imp.h"
#include "2D/ace.CoreEffectObject2D_Imp.h"
#include "2D/ace.CoreMapObject2D_Imp.h"
#include "2D/ace.CoreGeometryObject2D_Imp.h"
#include "2D/ace.CoreLayer2D_Imp.h"

#include "3D/ace.CoreLayer3D_Imp.h"
#include "3D/ace.CoreCameraObject3D_Imp.h"
#include "3D/ace.CoreDirectionalLightObject3D_Imp.h"
#include "3D/ace.CoreEffectObject3D_Imp.h"
#include "3D/ace.CoreModelObject3D_Imp.h"
#include "3D/ace.CoreMassModelObject3D_Imp.h"
#include "3D/ace.CoreTerrainObject3D_Imp.h"

#include "../Core/ace.Core_Imp.h"

#include "ace.CoreScene_Imp.h"

#include "PostEffect/ace.CorePostEffect_Imp.h"

#include "Transition/ace.CoreTransition_Imp.h"

#include "../Log/ace.Log_Imp.h"

#include "../Graphics/ace.Graphics_Imp.h"


#include "../Shape/ace.CoreCircleShape_Imp.h"
#include "../Shape/ace.CoreArcShape_Imp.h"
#include "../Shape/ace.CoreLineShape_Imp.h"
#include "../Shape/ace.CoreRectangleShape_Imp.h"
#include "../Shape/ace.CoreTriangleShape_Imp.h"

namespace ace
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
}