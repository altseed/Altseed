
#pragma once

#include "../asd.Core.Base.h"
#include "2D/asd.CoreObject2D.h"
#include "2D/asd.CoreTextureObject2D.h"
#include "2D/asd.CoreCameraObject2D.h"
#include "2D/asd.CoreTextObject2D.h"
#include "2D/asd.CoreEffectObject2D.h"
#include "2D/asd.CoreMapObject2D.h"
#include "2D/asd.CoreGeometryObject2D.h"
#include "2D/asd.CoreLayer2D.h"
#include "asd.CoreScene.h"

#include "../Shape/asd.CoreCircleShape.h"
#include "../Shape/asd.CoreArcShape.h"
#include "../Shape/asd.CoreLineShape.h"
#include "../Shape/asd.CoreRectangleShape.h"
#include "../Shape/asd.CoreTriangleShape.h"
#include "../Shape/asd.CorePolygonShape.h"

#include "Collision/2D/asd.CoreCircleCollider.h"
#include "Collision/2D/asd.CoreLineCollider.h"
#include "Collision/2D/asd.CoreRectangleCollider.h"
#include "Collision/2D/asd.CorePolygonCollider.h"

namespace asd
{
	class ObjectSystemFactory
	{
	public:
		virtual ~ObjectSystemFactory()
		{
		}

		virtual CoreChip2D* CreateChip2D() = 0;
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
		virtual CorePolygonShape* CreatePolygonShape() = 0;

		virtual CoreCircleCollider* CreateCircleCollider() = 0;
		virtual CoreLineCollider* CreateLineCollider() = 0;
		virtual CoreRectangleCollider* CreateRectangleCollider() = 0;
		virtual CorePolygonCollider* CreatePolygonCollider() = 0;
	};
}