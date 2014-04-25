
#include "ace.CubemapTexture_Imp_GL.h"
#include "../ace.Graphics_Imp_GL.h"

namespace ace
{
	CubemapTexture_Imp_GL::CubemapTexture_Imp_GL(Graphics* graphics, GLuint cubemapTexture)
		: CubemapTexture_Imp(graphics)
		, m_cubemapTexture(cubemapTexture)
	{

	}

	CubemapTexture_Imp_GL::~CubemapTexture_Imp_GL()
	{

	}

	CubemapTexture_Imp* CubemapTexture_Imp_GL::Create(Graphics_Imp* graphics, const achar* front, const achar* left, const achar* back, const achar* right, const achar* top, const achar* bottom)
	{
		auto loadFile = [](const achar* path, std::vector<uint8_t>& dst)-> bool
		{
#if _WIN32
			auto fp = _wfopen(path, L"rb");
			if (fp == nullptr) return false;
#else
			auto fp = fopen(ToUtf8String(path).c_str(), "rb");
			if (fp == nullptr) return false;
#endif
			fseek(fp, 0, SEEK_END);
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			dst.resize(size);
			fread(dst.data(), 1, size, fp);
			fclose(fp);

			return true;
		};

		int32_t widthes[6];
		int32_t heights[6];
		std::vector<uint8_t> fileBuffers[6];
		uint8_t* buffers[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		static const GLenum target[] = {
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		};

		const achar* pathes[] = {
			front,
			left,
			back,
			right,
			top,
			bottom,
		};

		for (int32_t i = 0; i < 6; i++)
		{
			if (!loadFile(pathes[i], fileBuffers[i]))
			{
				return nullptr;
			}
		}

		for (int32_t i = 0; i < 6; i++)
		{
			void* result = nullptr;
			if (ImageHelper::LoadPNGImage(fileBuffers[i].data(), fileBuffers[i].size(), true, widthes[i], heights[i], result))
			{
				buffers[i] = (uint8_t*) result;
			}
			else
			{
				goto End;
			}
		}


		auto width = widthes[0];
		auto height = heights[0];

		for (int32_t i = 0; i < 6; i++)
		{
			if (widthes[i] != width) goto End;
			if (heights[i] != height) goto End;
		}

		GLuint cubemapTexture = 0;

		glGenTextures(1, &cubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(target[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffers[i]);
		}

		for (int32_t i = 0; i < 6; i++)
		{
			SafeDeleteArray(buffers[i]);
		}
		return new CubemapTexture_Imp_GL(graphics, cubemapTexture);

	End:;

		for (int32_t i = 0; i < 6; i++)
		{
			SafeDeleteArray(buffers[i]);
		}

		return nullptr;
	}
}