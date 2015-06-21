
#pragma once

#include "../asd.Graphics_Imp.h"

// AnimationGif
#define NONDLL	1
#define MSWIN32 1
#define BGDWIN32 1
#include <gd/gd.h>
#include <gd/gdfontmb.h>

namespace asd {

	class GifAnimationHelper
	{

		FILE*		fp = nullptr;
		int32_t		width = 0;
		int32_t		height = 0;
		int32_t		framerate = 0;
		gdImagePtr	imgPtr = nullptr;

	public:
		bool Initialize(const achar* filepath, int32_t width, int32_t height, int32_t framerate, float scale);
		void AddImage(Color* data, int width, int height);
		void Finalize();
	};

}