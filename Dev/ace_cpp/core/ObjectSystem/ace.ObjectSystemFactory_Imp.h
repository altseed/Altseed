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

		CoreTextureObject2D* CreateTextureObject2D();
		CoreLayer2D* CreateLayer2D();

		CoreCameraObject3D* CreateCameraObject3D();
		CoreDirectionalLightObject3D* CreateDirectionalLightObject3D();
		CoreLayer3D* CreateLayer3D();

		CoreScene* CreateScene();
		CorePostEffect* CreatePostEffect();
	};
}