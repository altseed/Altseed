
#pragma once

#include "../ace.Core.Base.h"
#include "2D/ace.CoreObject2D.h"
#include "2D/ace.CoreTextureObject2D.h"
#include "2D/ace.CoreCameraObject2D.h"
#include "2D/ace.CoreTextObject2D.h"
#include "2D/ace.CoreEffectObject2D.h"
#include "2D/ace.CoreMapObject2D.h"
#include "2D/ace.CoreGeometryObject2D.h"
#include "2D/ace.CoreLayer2D.h"
#include "ace.CoreScene.h"

#include "../Shape/ace.CoreCircleShape.h"
#include "../Shape/ace.CoreArcShape.h"
#include "../Shape/ace.CoreLineShape.h"
#include "../Shape/ace.CoreRectangleShape.h"
#include "../Shape/ace.CoreTriangleShape.h"

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
		virtual CoreTextObject2D* CreateTextObject2D() = 0;
		virtual CoreEffectObject2D* CreateEffectObject2D() = 0;
		virtual CoreMapObject2D* CreateMapObject2D() = 0;
		virtual CoreGeometryObject2D* CreateGeometryObject2D() = 0;
		virtual CoreLayer2D* CreateLayer2D() = 0;


		virtual CoreEffectObject3D* CreateEffectObject3D() = 0;
		virtual CoreModelObject3D* CreateModelObject3D() = 0;
		virtual CoreCameraObject3D* CreateCameraObject3D() = 0;
		virtual CoreDirectionalLightObject3D* CreateDirectionalLightObject3D() = 0;
		virtual CoreMassModelObject3D* CreateCoreMassModelObject3D() = 0;
		virtual CoreTerrainObject3D* CreateCoreTerrainObject3D() = 0;
		virtual CoreLayer3D* CreateLayer3D(RenderSettings settings) = 0;

		virtual CoreScene* CreateScene() = 0;
		virtual CorePostEffect* CreatePostEffect() = 0;

		virtual CoreTransition* CreateTransition() = 0;

		virtual CoreArcShape* CreateArcShape() = 0;
		virtual CoreCircleShape* CreateCircleShape() = 0;
		virtual CoreLineShape* CreateLineShape() = 0;
		virtual CoreRectangleShape* CreateRectangleShape() = 0;
		virtual CoreTriangleShape* CreateTriangleShape() = 0;
	};
}