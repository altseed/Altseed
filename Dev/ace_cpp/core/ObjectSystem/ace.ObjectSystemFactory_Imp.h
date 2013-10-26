#pragma once
#include "ace.ObjectSystemFactory.h"

namespace ace
{
	class ObjectSystemFactory_Imp : public ObjectSystemFactory
	{
	private:
		Graphics_Imp* m_graphics;
		Log_Imp* m_logger;
		Vector2DI m_windowSize;

	public:
		ObjectSystemFactory_Imp(Graphics_Imp* graphics, Log_Imp* logger, Vector2DI windowSize);
		virtual ~ObjectSystemFactory_Imp();

		CoreObject2D* CreateObject2D();
		CoreTextureObject2D* CreateTextureObject2D();
		CoreLayer2D* CreateLayer2D();
		CoreScene* CreateScene();
		CorePostEffect* CreatePostEffect();
	};
}