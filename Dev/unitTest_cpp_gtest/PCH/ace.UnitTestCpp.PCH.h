
#pragma once

#include <ace.common.Base.h>
#include <gtest/gtest.h>


#include <Math/ace.Vector2DF.h>
#include <Math/ace.Vector3DF.h>
#include <Math/ace.Matrix44.h>

#include <Window/ace.Window_Imp.h>
#include <Log/ace.Log_Imp.h>

#include <Graphics/ace.Graphics_Imp.h>
#include <Graphics/Resource/ace.VertexBuffer_Imp.h>
#include <Graphics/Resource/ace.IndexBuffer_Imp.h>
#include <Graphics/Resource/ace.NativeShader_Imp.h>
#include <Graphics/Resource/ace.RenderTexture2D_Imp.h>
#include <Graphics/Resource/ace.DepthBuffer_Imp.h>
#include <Graphics/Resource/ace.CubemapTexture_Imp.h>

#include <cstdint>
#include <iostream>

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

	static std::vector<uint8_t> GetBinaryDataAsUint8(astring filePath)
	{
		FILE* fp = nullptr;

#if _WIN32
		_wfopen_s(&fp, filePath.c_str(), L"rb");
		if (fp == nullptr) return std::vector<uint8_t>();
#else
		fp = fopen(ToUtf8String(filePath.c_str()).c_str(), "rb");
		if (fp == nullptr) return std::vector<uint8_t>();
#endif

		fseek(fp, 0, SEEK_END);
		auto size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::vector<uint8_t> data;
		data.resize(size);

		fread(&(data[0]), 1, size, fp);
		fclose(fp);

		return data;
	}
}