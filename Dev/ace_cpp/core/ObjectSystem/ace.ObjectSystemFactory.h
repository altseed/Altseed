
#pragma once

#include "2D/ace.CoreObject2D.h"
#include "2D/ace.CoreTextureObject2D.h"
#include "2D/ace.CoreCameraObject2D.h"
#include "2D/ace.CoreLayer2D.h"
#include "ace.CoreScene.h"

namespace ace
{
	class ObjectSystemFactory
	{
	public:
		virtual ~ObjectSystemFactory()
		{
		}

		virtual CoreTextureObject2D* CreateTextureObject2D() = 0;
		virtual CoreCameraObject2D* CreateCameraObject2D() = 0;
		virtual CoreLayer2D* CreateLayer2D() = 0;


		virtual CoreEffectObject3D* CreateEffectObject3D() = 0;
		virtual CoreModelObject3D* CreateModelObject3D() = 0;
		virtual CoreCameraObject3D* CreateCameraObject3D() = 0;
		virtual CoreDirectionalLightObject3D* CreateDirectionalLightObject3D() = 0;
		virtual CoreLayer3D* CreateLayer3D(RenderSettings settings) = 0;

		virtual CoreScene* CreateScene() = 0;
		virtual CorePostEffect* CreatePostEffect() = 0;
	};
}