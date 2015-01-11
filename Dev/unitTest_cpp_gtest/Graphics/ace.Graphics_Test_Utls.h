
#pragma once

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "../PCH/ace.UnitTestCpp.PCH.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void SaveSceenShot_(ace::Graphics_Imp* g, const char* file, int32_t num);

std::string GetFileName(const char* src);

void SetGLEnable(bool value);

void StartGraphicsTest();

#define SAVE_SCREEN_SHOT(g,num) { SaveSceenShot_(g,__FILE__, num); }

namespace ace
{
	static std::vector<int8_t> GetBinaryData(astring filePath)
	{
		FILE* fp = nullptr;

#if _WIN32
		_wfopen_s(&fp, filePath.c_str(), L"rb");
		if (fp == nullptr) return std::vector<int8_t>();
#else
		fp = fopen(ToUtf8String(filePath.c_str()).c_str(), "rb");
		if (fp == nullptr) return std::vector<int8_t>();
#endif

		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::vector<int8_t> data;
		data.resize(size);

		fread(&(data[0]), 1, size, fp);
		fclose(fp);

		return data;
	}
}