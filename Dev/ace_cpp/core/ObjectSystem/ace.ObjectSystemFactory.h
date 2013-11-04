#pragma once
#include "2D/ace.CoreObject2D.h"
#include "2D/ace.CoreTextureObject2D.h"
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
		virtual CoreLayer2D* CreateLayer2D() = 0;
		virtual CoreScene* CreateScene() = 0;
		virtual CorePostEffect* CreatePostEffect() = 0;
	};
}