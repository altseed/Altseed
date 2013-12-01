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

		CoreTextureObject2D* CreateTextureObject2D() override;
		CoreLayer2D* CreateLayer2D() override;

		CoreMeshObject3D* CreateMeshObject3D() override;
		CoreCameraObject3D* CreateCameraObject3D() override;
		CoreDirectionalLightObject3D* CreateDirectionalLightObject3D() override;
		CoreLayer3D* CreateLayer3D() override;

		CoreScene* CreateScene() override;
		CorePostEffect* CreatePostEffect() override;
	};
}