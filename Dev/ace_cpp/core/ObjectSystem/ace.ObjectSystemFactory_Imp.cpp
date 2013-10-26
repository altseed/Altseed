#include "ace.ObjectSystemFactory_Imp.h"
#include "2D/ace.CoreObject2D_Imp.h"
#include "2D/ace.CoreTextureObject2D_Imp.h"
#include "2D/ace.CoreLayer2D_Imp.h"
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
	CoreObject2D* ObjectSystemFactory_Imp::CreateObject2D()
	{
		return new CoreObject2D_Imp();
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

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CoreScene* ObjectSystemFactory_Imp::CreateScene()
	{
		return new CoreScene_Imp();
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	CorePostEffect* ObjectSystemFactory_Imp::CreatePostEffect()
	{
		return new CorePostEffect_Imp(m_graphics);
	}
}