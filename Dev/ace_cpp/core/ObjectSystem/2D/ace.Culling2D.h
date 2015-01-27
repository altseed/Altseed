#pragma once

#include <cstdio>
#include <Culling2D.h>
#include "../2D/ace.CoreObject2D.h"
#include "../../Graphics/Resource/ace.Chip2D.h"

#if _DEBUG
#pragma comment(lib,"Debug/Culling2D.lib")
#else
#pragma comment(lib,"Release/Culling2D.lib")
#endif

#define __CULLING_2D__ 1

namespace ace
{
	class Culling2DUserData
	{
	public:
		bool IsObject;
		CoreObject2D* Object;
		Chip2D* Chip;

		~Culling2DUserData()
		{
#if _DEBUG
			printf("Erased.\n");
#endif
		}

		Culling2DUserData()
		{
			IsObject = false;

			Object = nullptr;
			Chip = nullptr;

#if _DEBUG
			printf("Created.\n");
#endif
		}

		Culling2DUserData(CoreObject2D *mapObject,Chip2D* chip)
		{
			IsObject = false;

			Object = mapObject;
			Chip = chip;

#if _DEBUG
			printf("Created.\n");
#endif
		}

		Culling2DUserData(CoreObject2D *object)
		{
			IsObject = true;

			Object = object;
			Chip = nullptr;

#if _DEBUG
			printf("Created.\n");
#endif
		}
	};
}