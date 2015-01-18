#pragma once

#include <Culling2D.h>
#include "../2D/ace.CoreObject2D.h"
#include "../../Graphics/Resource/ace.Chip2D.h"

#if _DEBUG
#pragma comment(lib,"Debug/Culling2D.lib")
#else
#pragma comment(lib,"Release/Culling2D.lib")
#endif

#define __CULLING_2D__ 0

namespace ace
{
	struct Culling2DUserData
	{
		bool IsObject;
		CoreObject2D* Object;
		Chip2D* Chip;

		Culling2DUserData(CoreObject2D *mapObject,Chip2D* chip)
		{
			IsObject = false;

			Object = mapObject;
			Chip = chip;
		}

		Culling2DUserData(CoreObject2D *mapObject)
		{
			IsObject = true;

			Object = mapObject;
			Chip = nullptr;
		}
	};
}