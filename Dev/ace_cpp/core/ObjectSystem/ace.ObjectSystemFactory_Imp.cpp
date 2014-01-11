
#include "ace.ObjectSystemFactory_Imp.h"

#include "2D/ace.CoreTextureObject2D_Imp.h"
#include "2D/ace.CoreLayer2D_Imp.h"

#include "3D/ace.CoreLayer3D_Imp.h"
#include "3D/ace.CoreCameraObject3D_Imp.h"
#include "3D/ace.CoreDirectionalLightObject3D_Imp.h"
#include "3D/ace.CoreModelObject3D_Imp.h"

#include "ace.CoreScene_Imp.h"

#include "PostEffect/ace.CorePostEffect_Imp.h"

#include "../Log/ace.Log_Imp.h"

#include "../Graphics/Common/ace.Graphics_Imp.h"

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	ObjectSystemFactory_Imp::ObjectSystemFactory_Imp(Graphics_Imp* graphics, Log_Imp* logger, Vector2DI windowSize)
		: m_graphics(graphics)
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
		return new CoreTextureObject2D_Imp();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreLayer2D* ObjectSystemFactory_Imp::CreateLayer2D()
	{
		return new CoreLayer2D_Imp(m_graphics, m_logger, m_windowSize);
	}

	CoreModelObject3D* ObjectSystemFactory_Imp::CreateModelObject3D()
	{
		return new CoreModelObject3D_Imp(m_graphics);
	}

	CoreCameraObject3D* ObjectSystemFactory_Imp::CreateCameraObject3D()
	{
		return new CoreCameraObject3D_Imp(m_graphics);
	}

	CoreDirectionalLightObject3D* ObjectSystemFactory_Imp::CreateDirectionalLightObject3D()
	{
		return new CoreDirectionalLightObject3D_Imp(m_graphics);
	}

	CoreLayer3D* ObjectSystemFactory_Imp::CreateLayer3D()
	{
		return new CoreLayer3D_Imp(m_graphics, m_logger, m_windowSize);
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
}